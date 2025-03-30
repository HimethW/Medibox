//This file contains utility functions for displaying information on an OLED screen using the Adafruit SSD1306 library.

#include "display_utils.h"  // Include the header file

void print_line(String message, int column, int row, int textSize, bool selected) {
  display.setTextSize(textSize);
  if (selected) {
    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds(message, column, row, &x1, &y1, &w, &h);        //if the menu option is selected, calculate the text bounds to draw a rectangle behind the text 

    // Draw a white rectangle behind the text (highlight effect)
    display.fillRect(column, row, w+1, h+1, SSD1306_WHITE);               //fill the rectangle with white color
    display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);                   // Black text, white background
  } else {
    display.setTextColor(SSD1306_WHITE);                                  // White text, black background
  }
  display.setCursor(column+1, row+1);
  display.println(message);                                               // Print the message with a small offset to center it in the rectangle

  if(homeScreen){                                                         //Only display temperature and Humidity if we are on the hme screen
    display.setTextSize(1);
    if(tempHigh){
      display.setCursor(0,40);
      display.println("Temp high: "+ String(temp,1));                    //Display the temperature and humidity values with one decimal place
    }else if(tempLow){
      display.setCursor(0,40);
      display.println("Temp low: "+ String(temp,1));
    }else{
      display.setCursor(0,40);
      display.println("Temp normal: "+ String(temp,1));
    }
    if(humidityHigh){
      display.setCursor(0,50);
      display.println("Humidity high: "+ String(humidity,1) + "%");
    }else if(humidityLow){
      display.setCursor(0,50);
      display.println("Humidity low: "+ String(humidity,1) + "%");
    }else{
      display.setCursor(0,50);
      display.println("Humidity normal: "+ String(humidity,1) + "%");
    }
  }
  

  display.display();
  }