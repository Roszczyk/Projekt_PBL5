#include "water_sensors.h"

gpio_t echo_pin = GPIO_PIN(PORT_A, 9);
gpio_t trigger_pin = GPIO_PIN(PORT_B, 12);
static srf04_t distance_sensor;

// USER CONFIG:
uint16_t sensor_height_cm = 100; // TODO set in config
int32_t PH_ADC_5V_VALUE = 6000;
int32_t PH_7_OFFSET = 38;


static int get_distance(void);


uint8_t init_periphs(void)
{
	DEBUG("Initializing peripherials...\n\r");

	srf04_params_t dist_sen_params = {trigger_pin, echo_pin};
	int ret = srf04_init(&distance_sensor, &dist_sen_params);

	if(ret == SRF04_OK)
		DEBUG("ultrasonic sensor - OK\n\r");
	else
	{
		DEBUG("ultrasonic sensor - FAIL\n\r");
		return 1;
	}

	ret = adc_init(ADC_LINE(PH_SENSOR_ADC_LINE));

	if(ret == 0)
		DEBUG("pH sensor - OK\n\r");
	else
	{
		DEBUG("pH sensor - FAIL\n\r");
		return 2;
	}

	return 0;
}

static int get_distance(void)
{
    return srf04_get_distance(&distance_sensor)/10;
}

uint16_t get_water_level(void)
{
	int dist = get_distance();

	if(dist <= 0 || dist > 65535)
	{
		DEBUG("Distance sensor value error\n\r");
		return 0;
	}

	uint16_t level = 0;

	if(sensor_height_cm > dist)
	{
		level = sensor_height_cm - dist;
	}

	return level;
}

uint8_t get_ph_level(void)
{
	int32_t adc_value = adc_sample(ADC_LINE(PH_SENSOR_ADC_LINE),ADC_RES_10BIT);

	int pH_val = adc_value * 35 * 5 / PH_ADC_5V_VALUE + PH_7_OFFSET;

	uint8_t ph_normalized = 0;

	if(pH_val < 0)
	{
		ph_normalized = 0;
		DEBUG("pH value error (<0)!\n\r");
	}
	else if(pH_val > 140)
	{
		ph_normalized = 140;
		DEBUG("pH value error (>14.0)!\n\r");
	}
	else
	{
		ph_normalized = (uint8_t) pH_val;
	}

	DEBUG("pH meaurement:\t raw = %ld \t|\t pH = %d => %d\n\r", adc_value, pH_val, ph_normalized);

	return ph_normalized;
}