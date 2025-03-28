#include "display_utils.h"  // Include the header file

void print_line(String message, int column, int row, int textSize, bool selected) {
  display.setTextSize(textSize);
  if (selected) {
    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds(message, column, row, &x1, &y1, &w, &h);

    // Draw a white rectangle behind the text (highlight effect)
    display.fillRect(column, row, w+1, h+1, SSD1306_WHITE);
    display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);  // Black text, white background
  } else {
    display.setTextColor(SSD1306_WHITE);  // White text, black background
  }
  display.setCursor(column+1, row+1);
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