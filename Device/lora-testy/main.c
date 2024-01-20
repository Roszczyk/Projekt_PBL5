/*
 * Copyright (C) 2018 Inria
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     examples
 * @{
 *
 * @file
 * @brief       Example demonstrating the use of LoRaWAN with RIOT
 *
 * @author      Alexandre Abadie <alexandre.abadie@inria.fr>
 *
 * @}
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "msg.h"
#include "thread.h"
#include "fmt.h"

#include "board.h"       /* board specific definitions */
#include "periph/gpio.h" /* gpio api */
#include "stm32l072xx.h" /* mcu specific definitions */

#include "periph/pm.h"
#include "xtimer.h"
#if IS_USED(MODULE_PERIPH_RTC)
#include "periph/rtc.h"
#else
#include "timex.h"
#include "ztimer.h"
#endif

#include "net/loramac.h"
#include "semtech_loramac.h"

#define USER_BUTTON       (BTN0_PIN)

/* By default, messages are sent every 20s to respect the duty cycle
   on each channel */
#ifndef SEND_PERIOD_S
#define SEND_PERIOD_S       (20U)
#endif

/* Low-power mode level */
#define PM_LOCK_LEVEL       (1)

#define SENDER_PRIO         (THREAD_PRIORITY_MAIN - 1)
static kernel_pid_t sender_pid;
static char sender_stack[THREAD_STACKSIZE_MAIN / 2];

extern semtech_loramac_t loramac;
#if !IS_USED(MODULE_PERIPH_RTC)
static ztimer_t timer;
#endif

//static const char *message = "This is RIOT!";

#ifdef USE_OTAA
static uint8_t deveui[LORAMAC_DEVEUI_LEN];
static uint8_t appeui[LORAMAC_APPEUI_LEN];
static uint8_t appkey[LORAMAC_APPKEY_LEN];
#endif

#ifdef USE_ABP
static uint8_t devaddr[LORAMAC_DEVADDR_LEN];
static uint8_t nwkskey[LORAMAC_NWKSKEY_LEN];
static uint8_t appskey[LORAMAC_APPSKEY_LEN];
#endif

// uint32_t base64ToBinary(const char *base64)
// {
//     unsigned char binary[15] = {0};
//     char decodingTable[256];
//     int i, j;

//     for (i = 0; i < 64; ++i)
//         decodingTable["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] = i;

//     int base64Len = strlen(base64);
//     int binaryIndex = 0;

//     for (i = 0; i < base64Len; i += 4)
//     {
//         unsigned int sextet1 = decodingTable[base64[i]];
//         unsigned int sextet2 = decodingTable[base64[i + 1]];
//         unsigned int sextet3 = decodingTable[base64[i + 2]];
//         unsigned int sextet4 = decodingTable[base64[i + 3]];

//         unsigned int triple = (sextet1 << 18) | (sextet2 << 12) | (sextet3 << 6) | sextet4;

//         for (j = 2; j >= 0 && binaryIndex < 100; --j)
//         {
//             binary[binaryIndex++] = (triple >> (8 * j)) & 0xFF;
//         }
//     }
//     uint32_t a = *(uint32_t *)binary;

//     return a;
// }


#define LORAMAC_RECV_MSG_QUEUE                   (4U)
static msg_t _loramac_recv_queue[LORAMAC_RECV_MSG_QUEUE];
static char _recv_stack[THREAD_STACKSIZE_DEFAULT];

static void *_wait_recv(void *arg)
{
    msg_init_queue(_loramac_recv_queue, LORAMAC_RECV_MSG_QUEUE);

    (void)arg;
    while (1) {
        /* blocks until something is received */
        switch (semtech_loramac_recv(&loramac)) {
            case SEMTECH_LORAMAC_RX_DATA:
                loramac.rx_data.payload[loramac.rx_data.payload_len] = 0;
                printf("Data received: %s, port: %d\n",
                (char *)loramac.rx_data.payload, loramac.rx_data.port);
                break;

            case SEMTECH_LORAMAC_RX_LINK_CHECK:
                printf("Link check information:\n"
                   "  - Demodulation margin: %d\n"
                   "  - Number of gateways: %d\n",
                   loramac.link_chk.demod_margin,
                   loramac.link_chk.nb_gateways);
                break;

            case SEMTECH_LORAMAC_RX_CONFIRMED:
                puts("Received ACK from network");
                break;

            case SEMTECH_LORAMAC_TX_SCHEDULE:
                puts("The Network Server has pending data");
                break;

            default:
                break;
        }
    }
    return NULL;
}

// static void _alarm_cb(void *arg)
// {
//     (void)arg;
//     msg_t msg;
//     msg.content.value=2137;
//     msg_send(&msg, sender_pid);
// }

// static void _prepare_next_alarm(void)
// {
// #if IS_USED(MODULE_PERIPH_RTC)
//     struct tm time;
//     rtc_get_time(&time);
//     /* set initial alarm */
//     time.tm_sec += SEND_PERIOD_S;
//     mktime(&time);
//     rtc_set_alarm(&time, _alarm_cb, NULL);
// #else
//     timer.callback = _alarm_cb;
//     ztimer_set(ZTIMER_MSEC, &timer, SEND_PERIOD_S * MS_PER_SEC);
// #endif
// }
    
static void _send_message(uint32_t val)
{
    static uint32_t tmp;
    printf("Sending: %ld\r\n", val);
    /* Try to send the message */
    tmp = ztimer_now(ZTIMER_MSEC);
    uint8_t ret = semtech_loramac_send(&loramac,
                                       (uint8_t *) &val, 4);
    tmp = ztimer_now(ZTIMER_MSEC) - tmp;
    printf("Sending time: %ld\r\n", tmp);

    if (ret != SEMTECH_LORAMAC_TX_DONE) {
        printf("Cannot send message '%ld', ret code: %d\n", val, ret);
        return;
    }
}

// void user_button_callback(void* args)
// {
// 	(void)args;
//static xtimer_ticks32_t tmp;


// 	msg_t msg;

//     msg_send(&msg, sender_pid);
// }

void user_button_callback(void* args)
{
    static xtimer_ticks32_t tmp;
	(void)args;
	//static uint32_t tmp;

	msg_t msg;

	if(!gpio_read(USER_BUTTON))
	{
		tmp = ztimer_now(ZTIMER_MSEC);
	}
	else
	{
		tmp = (ztimer_now(ZTIMER_MSEC) - tmp);///1000;
		msg.content.value = tmp;
		msg_send(&msg, sender_pid);
        printf("Sending1: %ld\r\n", msg.content.value);
	}
}

static void *sender(void *arg)
{
    (void)arg;

    msg_t msg;
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    while (1) {
        msg_receive(&msg);

        /* Trigger the message send */
        _send_message(msg.content.value);

        /* Schedule the next wake-up alarm */
        //_prepare_next_alarm();
    }

    /* this should never be reached */
    return NULL;
}

int main(void)
{
    static uint32_t tmp;
    ztimer_acquire(ZTIMER_MSEC);
    puts("LoRaWAN Class A low-power application");
    puts("=====================================");

    /*
     * Enable deep sleep power mode (e.g. STOP mode on STM32) which
     * in general provides RAM retention after wake-up.
     */
#if IS_USED(MODULE_PM_LAYERED)
    for (unsigned i = 1; i < PM_NUM_MODES - 1; ++i) {
        pm_unblock(i);
    }
#endif

#ifdef USE_OTAA /* OTAA activation mode */
    /* Convert identifiers and keys strings to byte arrays */
	//semtech_loramac_init(&loramac);

    fmt_hex_bytes(deveui, CONFIG_LORAMAC_DEV_EUI_DEFAULT);
    fmt_hex_bytes(appeui, CONFIG_LORAMAC_APP_EUI_DEFAULT);
    fmt_hex_bytes(appkey, CONFIG_LORAMAC_APP_KEY_DEFAULT);
    


/*        printf("\nDEVEUI: ");
        for(int i=0; i<(int)(sizeof(deveui)/sizeof(deveui[0])); ++i) printf("%c", (char)deveui[i]);
        printf("\n APPEUI: ");
        for(int i=0; i<(int)(sizeof(appeui)/sizeof(appeui[0])); ++i) printf("%c", (char)appeui[i]);
        printf("\n APPKEY: ");
        for(int i=0; i<(int)(sizeof(appkey)/sizeof(appkey[0])); ++i) printf("%c", (char)appkey[i]);
        printf("\n");							*/


	semtech_loramac_set_deveui(&loramac, deveui);
    semtech_loramac_set_appeui(&loramac, appeui);
    semtech_loramac_set_appkey(&loramac, appkey);

    /* Use a fast datarate, e.g. BW125/SF7 in EU868 */
    semtech_loramac_set_dr(&loramac, LORAMAC_DR_5);

    /* Join the network if not already joined */
    if (!semtech_loramac_is_mac_joined(&loramac)) {
        /* Start the Over-The-Air Activation (OTAA) procedure to retrieve the
         * generated device address and to get the network and application session
         * keys.
         */
        puts("Starting join procedure");
        tmp = ztimer_now(ZTIMER_MSEC);
        if (semtech_loramac_join(&loramac, LORAMAC_JOIN_OTAA) != SEMTECH_LORAMAC_JOIN_SUCCEEDED) {
            puts("Join procedure failed");
            return 1;
        }
        tmp = ztimer_now(ZTIMER_MSEC)-tmp;
        printf("Join time: %ld\r\n", tmp);


#ifdef MODULE_PERIPH_EEPROM
        /* Save current MAC state to EEPROM */
        semtech_loramac_save_config(&loramac);
#endif
    }
#endif

#ifdef USE_ABP /* ABP activation mode */
    /* Convert identifiers and keys strings to byte arrays */
    fmt_hex_bytes(devaddr, CONFIG_LORAMAC_DEV_ADDR_DEFAULT);
    fmt_hex_bytes(nwkskey, CONFIG_LORAMAC_NWK_SKEY_DEFAULT);
    fmt_hex_bytes(appskey, CONFIG_LORAMAC_APP_SKEY_DEFAULT);
    semtech_loramac_set_devaddr(&loramac, devaddr);
    semtech_loramac_set_nwkskey(&loramac, nwkskey);
    semtech_loramac_set_appskey(&loramac, appskey);

    /* Configure RX2 parameters */
    semtech_loramac_set_rx2_freq(&loramac, CONFIG_LORAMAC_DEFAULT_RX2_FREQ);
    semtech_loramac_set_rx2_dr(&loramac, CONFIG_LORAMAC_DEFAULT_RX2_DR);

#ifdef MODULE_PERIPH_EEPROM
    /* Store ABP parameters to EEPROM */
    semtech_loramac_save_config(&loramac);
#endif

    /* Use a fast datarate, e.g. BW125/SF7 in EU868 */
    semtech_loramac_set_dr(&loramac, LORAMAC_DR_5);

    /* ABP join procedure always succeeds */
    semtech_loramac_join(&loramac, LORAMAC_JOIN_ABP);
#endif
    puts("Join procedure succeeded");

    /* start the sender thread */
    sender_pid = thread_create(sender_stack, sizeof(sender_stack),
                               SENDER_PRIO, 0, sender, NULL, "sender");

    thread_create(_recv_stack, sizeof(_recv_stack),
                  THREAD_PRIORITY_MAIN - 1, 0, _wait_recv, NULL, "recv thread");
    
    gpio_init_int(USER_BUTTON, GPIO_IN, GPIO_BOTH, user_button_callback, NULL);

    /* trigger the first send */
    // msg_t msg;
    // msg.content.value = 8;
    // msg_send(&msg, sender_pid);
    printf("HELLO\nSending: %d\n", 781234);
    return 0;
}
