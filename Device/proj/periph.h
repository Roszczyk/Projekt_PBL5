#ifndef PERIPH_H
#define PERIPH_H
#include "common.h"

float randomInRange(float min, float max);

bool getTempHum(void);
bool getGPS(void);

void setHeating(bool state);
void setLights(bool state);

#endif // PERIPH_H