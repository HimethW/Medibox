#include "sensor_utils.h"
#include "config.h"

void check_temp(){
    tempHigh = false;
    tempLow = false;
    humidityHigh = false;
    humidityLow = false; 
    TempAndHumidity data = dhtSensor.getTempAndHumidity();
    temp = data.temperature;
    humidity = data.humidity;
  
    if(temp > TEMP_UPPERLIMIT){
      tempHigh = true;
      digitalWrite(LED_TEMP, HIGH);
    }
    else if(temp < TEMP_LOWERLIMIT){
      tempLow = true;
      digitalWrite(LED_TEMP, HIGH);
    }
    else{
      digitalWrite(LED_TEMP, LOW);
    }
  
  
    if(humidity > HUMIDITY_UPPERLIMIT){
      humidityHigh = true;
      digitalWrite(LED_HUMIDITY, HIGH);
    }
    else if(humidity < HUMIDITY_LOWERLIMIT){
      humidityLow = true;
      digitalWrite(LED_HUMIDITY, HIGH);
    }
    else{
      digitalWrite(LED_HUMIDITY, LOW);
    }
  
  }