#ifndef PERIPH_H
#define PERIPH_H
#include "common.h"
#include "periph/gpio.h"
#include "fmt.h"
#include "dht.h"
#include "dht_params.h"

float randomInRange(float min, float max);

void initTemHum(void);
bool getTempHum(void);
bool getGPS(void);
void initSoundMove(gpio_t * pinSound, gpio_t * pinMove);
void getMoveSound(gpio_t * pinSound, gpio_t * pinMove);

void setHeating(bool state);
void setLights(bool state);

#endif // PERIPH_H
