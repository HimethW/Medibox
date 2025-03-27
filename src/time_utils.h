#ifndef TIME_UTILS_H
#define TIME_UTILS_H

#include <WiFi.h>           // For NTP time sync
#include "display_utils.h"  // For print_line()

// External declarations for global variables
extern int days;
extern int hours;
extern int minutes;
extern int seconds;
extern bool alarm_enabled[];
extern int n_alarms;
extern int alarm_hours[];
extern int alarm_minutes[];
extern bool alarm_triggered[];

// Function declarations
void update_time_with_check_alarm();
void update_time_wifi();
void print_time_now();

// External function declaration (used by update_time_with_check_alarm)
void ring_alarm();

#endif  // TIME_UTILS_H