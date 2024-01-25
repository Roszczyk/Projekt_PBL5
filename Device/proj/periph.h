#ifndef PERIPH_H
#define PERIPH_H
#include "common.h"
#include "periph/gpio.h"
#include "fmt.h"
#include "dht.h"
#include "dht_params.h"
#include "xtimer.h"

float randomInRange(float min, float max);

void initTemHum(dht_t * dht);
bool getTempHum(dht_t * dht);
bool getGPS(void);

void initSoundMove(void);
void getMoveSound(void);

void initSets(void);
void setHeating(bool state);
void setLights(bool state);

#endif // PERIPH_H
