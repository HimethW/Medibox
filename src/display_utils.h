#ifndef DISPLAY_UTILS_H  // Include guard
#define DISPLAY_UTILS_H

#include <Adafruit_SSD1306.h>  // Required for the display object

// External declarations for global variables used by print_line
extern Adafruit_SSD1306 display;  // The display object
extern bool tempHigh;             // Temperature and humidity status flags
extern bool tempLow;
extern bool humidityHigh;
extern bool humidityLow;

// Function declaration
void print_line(String message, int column, int row, int textSize, bool selected = false);

#endif  // DISPLAY_UTILS_H