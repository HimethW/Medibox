//This file contains utility functions for handling sensor data and displaying it

#include "sensor_utils.h"
#include "config.h"
#include "display_utils.h"

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

void check_light() {
  float LDR_voltage = analogRead(LDR_PIN);                      // Read the light value from the LDR
  number_of_samples++;                                          // Increment the number of samples
               
  float voltage = LDR_voltage / 4096. * 3.3;                    // Convert the analog reading to voltage(this equation was taken from the wokwi LDR documentation and changed to 3.3V of the ESP32)
  float resistance = 2000 * voltage / (1 - voltage / 3.3);
  float lux = pow(RL10 * 1e3 * pow(10, GAMMA_LDR) / resistance, (1 / GAMMA_LDR));
  total_lux_value += lux;                                       // Accumulate the lux value
                                 
  
}