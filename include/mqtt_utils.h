#ifndef MQTT_UTILS_H
#define MQTT_UTILS_H

#include <WiFi.h>
#include <PubSubClient.h>

// Declare external variables
extern WiFiClient espClient;
extern PubSubClient mqttClient;
extern int sampling_interval;
extern int sending_interval;
extern float gamma_factor;
extern int theta_offset;
extern int T_med;

// Function declarations
void setupMqtt();
void connectToBroker();
void mqttCallback(char* topic, byte* payload, unsigned int length);
void publish_data(const char* topic, float value);

#endif