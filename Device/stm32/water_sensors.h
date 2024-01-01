#ifndef __WATER_SENSORS_H__
#define __WATER_SENSORS_H__
#include "board.h"       /* board specific definitions */
#include "periph/gpio.h" /* gpio api */
#include "stm32l072xx.h" /* mcu specific definitions */

#include "srf04.h"

#define ENABLE_DEBUG    (1)
#if ENABLE_DEBUG
#include "debug.h"
#endif

#include <stdlib.h>
#include "periph/adc.h"

#define PH_SENSOR_ADC_LINE 	    0

/**
 * @brief init sensors
 * 
 * @return uint8_t - 0 on success, 1 if ultrasonic sensor fail, 2 if pH sensor fail
 */
uint8_t init_periphs(void);


/**
 * @brief Get the water level (water_level_cm = sensor_height_cm - ultrasonic_sensor_measure)
 * 
 * @return uint16_t water level in cm
 */
uint16_t get_water_level(void);

/**
 * @brief Get the pH level
 * 
 * @return uint8_t pH level [in unit*10]
 */
uint8_t get_ph_level(void);

#endif