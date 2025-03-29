//This file contains utility functions for handling sensor data and displaying it

#include "sensor_utils.h"
#include "config.h"

void check_temp(){                                              // Function to check temperature and humidity using the DHT sensor
    tempHigh = false;
    tempLow = false;
    humidityHigh = false;
    humidityLow = false; 
    TempAndHumidity data = dhtSensor.getTempAndHumidity();      // Get temperature and humidity data from the DHT sensor
    temp = data.temperature;                                    // Store the temperature and humidity values in global variables
    humidity = data.humidity;
  
    if(temp > TEMP_UPPERLIMIT){                                 // If the temperature exceeds the upper limit, set the tempHigh flag and turn on the LED
      tempHigh = true;
      digitalWrite(LED_TEMP, HIGH);
    }
    else if(temp < TEMP_LOWERLIMIT){                            // If the temperature goes below the lower limit, set the tempLow flag and turn on the LED
      tempLow = true;
      digitalWrite(LED_TEMP, HIGH);
    }
    else{                                                       // If the temperature is within the normal range, turn off the LED
      digitalWrite(LED_TEMP, LOW);
    }
  
  
    if(humidity > HUMIDITY_UPPERLIMIT){                         // If the humidity exceeds the upper limit, set the humidityHigh flag and turn on the LED
      humidityHigh = true;
      digitalWrite(LED_HUMIDITY, HIGH);
    }
    else if(humidity < HUMIDITY_LOWERLIMIT){                    // If the humidity goes below the lower limit, set the humidityLow flag and turn on the LED
      humidityLow = true;
      digitalWrite(LED_HUMIDITY, HIGH);
    }
    else{                                                       // If the humidity is within the normal range, turn off the LED
      digitalWrite(LED_HUMIDITY, LOW);
    }
  
  }