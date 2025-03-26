#include "display_utils.h"  // Include the header file

void print_line(String message, int column, int row, int textSize){
    display.setTextSize(textSize);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(column,row);
    display.println(message);
  
    display.setTextSize(1);
    if(tempHigh){
      display.setCursor(0,40);
      display.println("Temperature high");
    }else if(tempLow){
      display.setCursor(0,40);
      display.println("Temperature low");
    }
    if(humidityHigh){
      display.setCursor(0,50);
      display.println("Humidity high");
    }else if(humidityLow){
      display.setCursor(0,50);
      display.println("Humidity low");
    }
  
    display.display();
  }