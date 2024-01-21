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
#include <stdbool.h>
#include <time.h>
#include <stdint.h>

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

#include "cayenne_lpp.h"

#include "common.h"
#include "periph.h"

// to be removed - testing core functionality without wireless transmission
// #define LORA_OFF

#define USER_BUTTON (BTN0_PIN)

#ifndef SEND_PERIOD_S
#define SEND_PERIOD_S (40U)
#endif

#define GPS_PERIOD_S (35U)
#define TEMPHUM_PERIOD_S (20U)
#define SENSOR_PERIOD_S (30U)

/* Low-power mode level */
#define PM_LOCK_LEVEL (1)

static kernel_pid_t sender_pid;
static char sender_stack[THREAD_STACKSIZE_MAIN / 2];

extern semtech_loramac_t loramac;
// #if !IS_USED(MODULE_PERIPH_RTC)
// static ztimer_t timer;
// #endif

cayenne_lpp_t lpp = {0};
dht_t dev;
gpio_t soundPin, movePin;
gpio_t lightPin, heatingPin;


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

#define LORAMAC_RECV_MSG_QUEUE (4U)
static msg_t _loramac_recv_queue[LORAMAC_RECV_MSG_QUEUE];
static char _recv_stack[THREAD_STACKSIZE_DEFAULT];

#define SENSOR_PRIO (THREAD_PRIORITY_MAIN - 2)
static char temphum_stack[THREAD_STACKSIZE_DEFAULT];
static char gps_stack[THREAD_STACKSIZE_DEFAULT];
static char sm_stack[THREAD_STACKSIZE_DEFAULT];

void triggerUplink(void)
{
    static msg_t msg;
    msg_send(&msg, sender_pid);
}

bool executeDecodePayload(uint8_t *payload, uint8_t len)
{
    // if (len == 2)
    // {
    //     switch (payload[0])
    //     {
    //     case 0x01:
    //         setLights(payload[1]);
    //         break;

    //     case 0x02:
    //         setHeating(payload[1]);
    //         break;

    //     default:
    //         ERR("Unknown command: %d", payload[0]);
    //         return PAM_ERR;
    //         break;
    //     }
        // return PAM_OK;
    // }

    if (strcmp((char*)payload, "L1")==0){
        setLights(TRUE);
    }
    else if (strcmp((char*)payload, "L0")==0){
        setLights(FALSE);
    }
    else if (strcmp((char*)payload, "H1")==0){
        setHeating(TRUE);
    }
    else if (strcmp((char*)payload, "H0")==0){
        setHeating(FALSE);
    }
    else return PAM_ERR

    return PAM_OK;
}

// THERAD GPS
static void *GPSReader(void *arg)
{
    (void)arg;
    ztimer_now_t last_wakeup = ztimer_now(ZTIMER_MSEC);

    while (1)
    {
        getGPS();

        ztimer_periodic_wakeup(ZTIMER_MSEC, &last_wakeup, GPS_PERIOD_S * MS_PER_SEC);
        last_wakeup = ztimer_now(ZTIMER_MSEC);
    }

    /* this should never be reached */
    return NULL;
}

// THERAD TempHum
static void *tempHumReader(void *arg)
{
    (void)arg;
    ztimer_now_t last_wakeup = ztimer_now(ZTIMER_MSEC);

    while (1)
    {
        getTempHum();
	puts("TempHumReader active");
        ztimer_periodic_wakeup(ZTIMER_MSEC, &last_wakeup, TEMPHUM_PERIOD_S * MS_PER_SEC);
        last_wakeup = ztimer_now(ZTIMER_MSEC);
    }

    /* this should never be reached */
    return NULL;
}

static void *SMReader(void *arg)
{
    (void)arg;
    int sound, move;
    ztimer_now_t last_wakeup = ztimer_now(ZTIMER_MSEC);
    
    while(1){
        getMoveSound(&sound, &move);
        printf("sound: %d, move: %d", sound, move);
        ztimer_periodic_wakeup(ZTIMER_MSEC, &last_wakeup, SENSOR_PERIOD_S * MS_PER_SEC);
        last_wakeup = ztimer_now(ZTIMER_MSEC);
    }
    return NULL;
}

// THERAD receiver
static void *_wait_recv(void *arg)
{
    msg_init_queue(_loramac_recv_queue, LORAMAC_RECV_MSG_QUEUE);
    (void)arg;

#ifdef LORA_OFF
    return NULL;
#endif
    initSets();

    while (1)
    {	printf("Listening\n");
        /* blocks until something is received */
        switch (semtech_loramac_recv(&loramac))
        {
        case SEMTECH_LORAMAC_RX_DATA:
            loramac.rx_data.payload[loramac.rx_data.payload_len] = 0;
            printf("Data received: %s, port: %d\n",
                   (char *)loramac.rx_data.payload, loramac.rx_data.port);

            print_hex(loramac.rx_data.payload, loramac.rx_data.payload_len);

            if (PAM_ERR == executeDecodePayload(loramac.rx_data.payload, loramac.rx_data.payload_len))
            {
                puts("Error while executing payload");
            }
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

static void _send_message(uint8_t *payload, uint8_t len)
{
    static ztimer_now_t tmp;
    printf("Sending: %s\r\n", payload);
    print_hex(payload, len);
    puts("");

#ifdef LORA_OFF
    return;
#endif

    /* Try to send the message */
    tmp = ztimer_now(ZTIMER_MSEC);
    uint8_t ret = semtech_loramac_send(&loramac,
                                       payload, len);
    tmp = ztimer_now(ZTIMER_MSEC) - tmp;
    printf("Sending time: %ld\r\n", tmp);

    if (ret != SEMTECH_LORAMAC_TX_DONE)
    {
        printf("Cannot send message '%s', ret code: %d\n", payload, ret);
    }
}

void user_button_callback(void *args)
{
    (void)args;
    static ztimer_now_t tmp;

    puts("Button pressed");

    if (!gpio_read(USER_BUTTON))
    {
        tmp = ztimer_now(ZTIMER_MSEC);

        cayenne_lpp_add_digital_input(&lpp, 0, true);

        triggerUplink();
    }
    else
    {
        tmp = (ztimer_now(ZTIMER_MSEC) - tmp); /// 1000;
        // msg.content.value = tmp;
        // msg_send(&msg, sender_pid);
        printf("Press time: %ld\r\n", tmp);
    }
}

// THERAD sender
static void *sender(void *arg)
{
    (void)arg;

    msg_t msg;
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    while (1)
    {
        // msg_receive(&msg);
        ztimer_msg_receive_timeout(ZTIMER_MSEC, &msg, SEND_PERIOD_S * MS_PER_SEC);

        /* Trigger the message send */
        _send_message(lpp.buffer, lpp.cursor);
        cayenne_lpp_reset(&lpp);
    }

    /* this should never be reached */
    return NULL;
}

int main(void)
{
#ifndef LORA_OFF
    static ztimer_now_t tmp;
#endif
    ztimer_acquire(ZTIMER_MSEC);
    puts("LoRaWAN Class A low-power application");
    puts("=====================================");

#ifdef LORA_OFF
    puts("LoRaWAN is disabled using #define LORA_OFF");
#endif

    /*
     * Enable deep sleep power mode (e.g. STOP mode on STM32) which
     * in general provides RAM retention after wake-up.
     */
#if IS_USED(MODULE_PM_LAYERED)
    for (unsigned i = 1; i < PM_NUM_MODES - 1; ++i)
    {
        pm_unblock(i);
    }
#endif

#ifdef USE_OTAA /* OTAA activation mode */
    /* Convert identifiers and keys strings to byte arrays */
    fmt_hex_bytes(deveui, CONFIG_LORAMAC_DEV_EUI_DEFAULT);
    fmt_hex_bytes(appeui, CONFIG_LORAMAC_APP_EUI_DEFAULT);
    fmt_hex_bytes(appkey, CONFIG_LORAMAC_APP_KEY_DEFAULT);
    semtech_loramac_set_deveui(&loramac, deveui);
    semtech_loramac_set_appeui(&loramac, appeui);
    semtech_loramac_set_appkey(&loramac, appkey);

    /* Use a fast datarate, e.g. BW125/SF7 in EU868 */
    semtech_loramac_set_dr(&loramac, LORAMAC_DR_5);
#ifndef LORA_OFF
    /* Join the network if not already joined */
    if (!semtech_loramac_is_mac_joined(&loramac))
    {
        /* Start the Over-The-Air Activation (OTAA) procedure to retrieve the
         * generated device address and to get the network and application session
         * keys.
         */
        puts("Starting join procedure");
        tmp = ztimer_now(ZTIMER_MSEC);

        if (semtech_loramac_join(&loramac, LORAMAC_JOIN_OTAA) != SEMTECH_LORAMAC_JOIN_SUCCEEDED)
        {
            puts("Join procedure failed");
            return 1;
        }
        tmp = ztimer_now(ZTIMER_MSEC) - tmp;
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

#endif

	// initTemHum();
    initSoundMove();

    /* start the sender thread */
    sender_pid = thread_create(sender_stack, sizeof(sender_stack),
                               THREAD_PRIORITY_MAIN - 1, 0, sender, NULL, "sender");

    thread_create(_recv_stack, sizeof(_recv_stack),
                  THREAD_PRIORITY_MAIN - 1, 0, _wait_recv, NULL, "receiver");

    thread_create(temphum_stack, sizeof(temphum_stack),
                  SENSOR_PRIO, 0, tempHumReader, NULL, "tempHumReader");

    thread_create(gps_stack, sizeof(gps_stack),
                  SENSOR_PRIO, 0, GPSReader, NULL, "GPSReader");

    thread_create(sm_stack, sizeof(sm_stack),
                  SENSOR_PRIO, 0, SMReader, NULL, "SMReader");

    gpio_init_int(USER_BUTTON, GPIO_IN, GPIO_BOTH, user_button_callback, NULL);

    return 0;
}
