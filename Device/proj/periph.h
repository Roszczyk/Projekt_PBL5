#ifndef PERIPH_H
#define PERIPH_H
#include "common.h"
#include "dht.h"
#include "dht_params.h"

float randomInRange(float min, float max);

void initTemHum(dht_t * dev);
bool getTempHum(void);
bool getGPS(void);

void setHeating(bool state);
void setLights(bool state);

#endif // PERIPH_H