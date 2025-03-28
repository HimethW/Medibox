#include "time_utils.h"

void print_time_now() {
  display.clearDisplay();
  String time = "Day: " + String(days) + "\n" + String(hours) + ":" + String(minutes) + ":" + String(seconds);
  homeScreen = true;
  print_line(time, 0, 0, 2);
  homeScreen = false;
}

void update_time_wifi() {
  struct tm timeinfo;
  getLocalTime(&timeinfo);
  days = timeinfo.tm_mday;
  hours = timeinfo.tm_hour;
  minutes = timeinfo.tm_min;
  seconds = timeinfo.tm_sec;
}

void update_time_with_check_alarm() {
  update_time_wifi();
  print_time_now();

  for (int i = 0; i < n_alarms; i++) {
    if (alarm_triggered_minutes[i] && alarm_triggered_minutes[i] < minutes) {
      alarm_triggered[i] = false;
    }

    if (alarm_enabled[i]) {
      if (alarm_hours[i] == hours && alarm_minutes[i] == minutes && !alarm_triggered[i]) {
        alarm_triggered[i] = true;
        alarm_triggered_minutes[i] = minutes;
        ring_alarm(i);
      }
    }
  }
}