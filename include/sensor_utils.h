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
extern float total_lux_value;
extern int number_of_samples;
extern int sampling_interval;//temp
extern int sending_interval;//temp


void check_temp();
void check_light();

#endif