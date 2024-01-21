#include "periph.h"

extern cayenne_lpp_t lpp;
extern dht_t dev;
extern gpio_t soundPin, movePin;
extern gpio_t lightPin, heatingPin;

float randomInRange(float min, float max)
{
    return min + ((float)rand() / RAND_MAX) * (max - min);
}

void initTemHum(void)
{
    dht_params_t my_params;
    my_params.pin=GPIO_PIN(PORT_A, 10);
    my_params.type=DHT11;
    my_params.in_mode=DHT_PARAM_PULL;
	
	puts("Initializing DHT");
	dht_init(&dev, &my_params);
	puts("Sensor initialized");

   if(dht_init(&dev, &my_params)==DHT_OK){
       printf("DHT sensor connected\n");
   }
   else{
       puts("Failed to connect to DHT sensor");
   }
}

void initSoundMove(void)
{
    soundPin=GPIO_PIN(PORT_A, 0);
    movePin=GPIO_PIN(PORT_A, 8);
    gpio_init(soundPin, GPIO_IN);
    gpio_init(movePin, GPIO_IN);
}

bool getTempHum(void)
{
    puts("getTempHum");

    //int16_t temp, hum;
    //if(dht_read(&dev, &temp, &hum)!=DHT_OK){
    //    printf("Error reading values\n");
    //}

    static const float baseTemperature = 27.2; // Example temperature
    static const float baseHumidity = 35.4;    // Example humidity
    static const float range = 1.0;            // Range of ±1

    float randomTemperature = randomInRange(baseTemperature - range, baseTemperature + range);
    float randomHumidity = randomInRange(baseHumidity - range, baseHumidity + range);

    float tempF, humF;
    //tempF=(float)temp/10;
    //humF=(float)hum/10;
	tempF=randomTemperature;
	humF=randomHumidity;
    cayenne_lpp_add_temperature(&lpp, 0, (int)tempF);
    cayenne_lpp_add_relative_humidity(&lpp, 0, (int)humF);
    printf("temperature: %f, humidity: %f\n", tempF, humF);

    return PAM_OK;
}

void getMoveSound(int * sound, int * move)
{
    *sound = gpio_read(soundPin);
    printf("Value read from the pin sound: %d\n", *sound);
    *move = gpio_read(soundPin);
    printf("Value read from the pin move: %d\n", *move);
    if (*sound > 0 || *move > 0) cayenne_lpp_add_presence(&lpp, 0, 0xFF);
    else if (*sound > 0 || *move == 0) cayenne_lpp_add_presence(&lpp, 0, 0xF0);
    else if (*sound == 0 || *move > 0) cayenne_lpp_add_presence(&lpp, 0, 0x0F);
    else cayenne_lpp_add_presence(&lpp, 0, 0x00);
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

void initSets(void){
    lightPin=GPIO_PIN(PORT_A, 3);
    heatingPin=GPIO_PIN(PORT_A, 4);
    gpio_init(lightPin, GPIO_OUT);
    gpio_init(heatingPin, GPIO_OUT);
}

void setHeating(bool state)
{
    if (state)
    {
        gpio_set(heatingPin);
        puts("Heating on");
    }
    else
    {
        gpio_clear(heatingPin);
        puts("Heating off");
    }
}

void setLights(bool state)
{
    if (state)
    {
        gpio_set(lightPin);
        puts("Lights on");
    }
    else
    {
        gpio_clear(lightPin);
        puts("Lights off");
    }
}
