/**
 * @ingroup     water
 * @{
 *
 * @file
 * @brief       water level and pH meter - PBL5 Z22
 *
 * @author      Karol Duszczyk
 * @author      Michal Ciesielski
 * @author      Julia Dasiewicz
 * @author      Adam Pomorski
 *
 * @}
 */

#include <stdio.h>

#include "board.h"		 /* board specific definitions */
#include "periph/gpio.h" /* gpio api */
#include "stm32l072xx.h" /* mcu specific definitions */

/* threading includes */
#include "thread.h"
#include "msg.h"
#include "xtimer.h"

#define ENABLE_DEBUG (1)
#if ENABLE_DEBUG
#include "debug.h"
#endif

#include <stdlib.h>
#include "periph/adc.h"
#include "shell.h"
#include "string.h"

// #include "periph/pm.h"

#include "water_lorawan.h"
// #include "water_sensors.h"

// #include "proto_interface.h"

// #define MODULE_PERIPH_GPIO_IRQ

/* button manipulation macro */
#define USER_BUTTON (BTN0_PIN)

/* led manipulation macros */
#define RED_LED_OFF (LED3_OFF)
#define RED_LED_ON (LED3_ON)
#define RED_LED_TOGGLE (LED3_TOGGLE)
#define BLUE_LED_OFF (LED2_OFF)
#define BLUE_LED_ON (LED2_ON)
#define BLUE_LED_TOGGLE (LED2_TOGGLE)
#define GREEN_LED_OFF (LED1_OFF)
#define GREEN_LED_ON (LED1_ON)
#define GREEN_LED_TOGGLE (LED1_TOGGLE)

/* leds period times (can be changed) */
#define RED_LED_PERIOD (250000)
#define GREEN_LED_PERIOD (1000000)
#define BLUE_LED_PERIOD (250000)

kernel_pid_t sensors_PID = 0;

// char stack_thread_sensors[THREAD_STACKSIZE_MAIN];
// char stack_thread_receiver[THREAD_STACKSIZE_MAIN];

uint32_t measure_delay_s = 10;

#define samples_number 5
#define sampling_delay_ms 100

// void *thread_sensors(void *arg)
// {
// 	(void)arg;

// 	xtimer_ticks32_t xtime = xtimer_now();

// 	while (1)
// 	{
// 		if (if_lorawan_joined() == 0)
// 		{
// 			join_lorawan();
// 		}

// 		uint16_t water_level_tab[samples_number] = {0};
// 		uint8_t pH_level_tab[samples_number] = {0};

// 		for (int i = 0; i < samples_number; i++)
// 		{
// 			water_level_tab[i] = get_water_level();
// 			pH_level_tab[i] = get_ph_level();
// 			DEBUG("%d cm, %d pH*10\n\r", water_level_tab[i], pH_level_tab[i]);
// 			xtimer_periodic_wakeup(&xtime, sampling_delay_ms * 1000);
// 		}

// 		// todo

// 		uint16_t water_level = 0;
// 		uint32_t pH_level_sum = 0;
// 		uint8_t pH_level = 0;

// 		for (int i = 0; i < samples_number; i++)
// 		{
// 			water_level += water_level_tab[i];
// 			pH_level_sum += (uint32_t)pH_level_tab[i];
// 		}

// 		water_level /= samples_number;
// 		pH_level = (uint8_t)(pH_level_sum / samples_number);

// 		DEBUG("Avg %d cm, %d pH*10\n\r", water_level, pH_level);

// 		struct UplinkMessage uplink_msg = {0};
// 		uplink_msg.water_level = water_level;
// 		uplink_msg.ph = pH_level;

// 		uint8_t len = 0;

// 		uint8_t *msg_tmp = encode_uplink(&uplink_msg, &len);

// 		DEBUG("Sending %d bytes:", len);
// 		for (int i = 0; i < len; i++)
// 		{
// 			DEBUG(" %02X", msg_tmp[i]);
// 		}

// 		send_lorawan_message(msg_tmp, len);

// 		DEBUG("Sleep for %lu s\n\r", measure_delay_s);

// 		xtimer_periodic_wakeup(&xtime, measure_delay_s * 1000000); // sleep us
// 	}
// 	return NULL;
// } // end of thread_sensors

int main(void)
{
	int status = 0; // init_periphs();

	if (status != 0)
		return 0;

	status = init_lorawan();
	status = join_lorawan();

	// sensors_PID = thread_create(stack_thread_sensors, sizeof(stack_thread_sensors),
	// 		THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, thread_sensors,
	// 		NULL, "sensors");

	// thread_create(stack_thread_receiver, sizeof(stack_thread_receiver),
	//               THREAD_PRIORITY_MAIN - 1, 0, receive_lorawan_message_thr, &measure_delay_s, "recv thread");

	return 0;
}
