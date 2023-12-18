#include "periph.h"

extern cayenne_lpp_t lpp;

float randomInRange(float min, float max)
{
    return min + ((float)rand() / RAND_MAX) * (max - min);
}

bool getTempHum(void)
{
    puts("getTempHum");
    static const float baseTemperature = 27.2; // Example temperature
    static const float baseHumidity = 35.4;    // Example humidity
    static const float range = 1.0;            // Range of ±1

    float randomTemperature = randomInRange(baseTemperature - range, baseTemperature + range);
    float randomHumidity = randomInRange(baseHumidity - range, baseHumidity + range);

    cayenne_lpp_add_temperature(&lpp, 0, randomTemperature);
    cayenne_lpp_add_relative_humidity(&lpp, 0, randomHumidity);

    return PAM_OK;
}

bool getGPS(void)
{
    puts("getGPS");
    static const float baseLatitude = 37.7749;    // Example latitude
    static const float baseLongitude = -122.4194; // Example longitude
    static const float range = 0.0002;            // Range of ±0.0002

    float randomLatitude = randomInRange(baseLatitude - range, baseLatitude + range);
    float randomLongitude = randomInRange(baseLongitude - range, baseLongitude + range);

    cayenne_lpp_add_gps(&lpp, 0, randomLatitude, randomLongitude, 0);

    return PAM_OK;
}

void setHeating(bool state)
{
    if (state)
    {
        // gpio_set(GPIOA, GPIO5);
        puts("Heating on");
    }
    else
    {
        // gpio_clear(GPIOA, GPIO5);
        puts("Heating off");
    }
}

void setLights(bool state)
{
    if (state)
    {
        // gpio_set(GPIOA, GPIO5);
        puts("Lights on");
    }
    else
    {
        // gpio_clear(GPIOA, GPIO5);
        puts("Lights off");
    }
}
