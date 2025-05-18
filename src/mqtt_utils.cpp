//this file contains the implementation of the MQTT functions
#include "mqtt_utils.h"
#include "config.h"
#include <Arduino.h>

void setupMqtt() {
  mqttClient.setServer("ora6.emanate.io", 1883);            // Set the MQTT server and port (this is an server i have and it runs the mosquitto MQTT service)
  mqttClient.setCallback(mqttCallback);                     // Set the callback function for incoming messages
}

void connectToBroker(){                                     // Function to connect to the MQTT broker
  while(!mqttClient.connected()){
    Serial.print("Attempting MQTT connection");
    if(mqttClient.connect("ESP32")){                        // Attempt to connect to the broker
      Serial.println("connected");
      mqttClient.subscribe("sample_interval");              // Subscribe to topics
      mqttClient.subscribe("send_interval");
      mqttClient.subscribe("gamma");
      mqttClient.subscribe("offset");
      mqttClient.subscribe("T_med");
    }else{
      Serial.print("Failed ");
      Serial.print(mqttClient.state());
      delay(2000);
    }
  }
}

void mqttCallback(char* topic, byte* payload, unsigned int length){     // Callback function to handle incoming messages
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print(" ]");

  char payloadCharAr[length+1];                                         // Create a character array to hold the payload
  for(int i = 0; i<length; i++){
    Serial.print((char)payload[i]);
    payloadCharAr[i] = (char)payload[i];
  }
  payloadCharAr[length] = '\0';                                         // Null-terminate the character array
  if (strcmp(topic, "sample_interval") == 0) {
    sampling_interval = atoi(payloadCharAr);                            // Convert the payload to an integer   
  }
  else if(strcmp(topic, "send_interval") == 0) {
    sending_interval = int(atof(payloadCharAr)*60);                     // Convert the payload(minutes) to an integer in seconds
  }else if(strcmp(topic, "gamma") == 0) {
    gamma_factor = atof(payloadCharAr);                                 // Convert the payload to a float
  }
  else if(strcmp(topic, "offset") == 0){
    theta_offset = atoi(payloadCharAr);                                 // Convert the payload to an integer
  }else if(strcmp(topic, "T_med") == 0){
    T_med = atoi(payloadCharAr);                                        // Convert the payload to an integer
  }
  Serial.println();

}

void publish_data(const char* topic , float value){
  char dataArray[8];                                                    // Create a character array to hold the string representation of the float value
  if(strcmp(topic, "Light_intensity") == 0){
    dtostrf(value, 1, 4, dataArray);                                    // Convert the float value to a string with 4 decimal places
  }else if(strcmp(topic, "servo_angle") == 0){
    dtostrf(value, 1, 2, dataArray);                                    // Convert the float value to a string with 2 decimal places
  }
                  
  mqttClient.publish(topic,dataArray);                                  // Publish the data to the specified topic
}