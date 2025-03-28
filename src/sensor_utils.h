#ifndef SENSOR_UTILS_H
#define SENSOR_UTILS_H

#include <DHTesp.h>

extern DHTesp dhtSensor;
extern bool tempHigh;
extern bool tempLow;
extern bool humidityHigh;
extern bool humidityLow;
extern float temp;
extern float humidity;

void check_temp();

#endif