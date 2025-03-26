#include "time_utils.h"

void print_time_now() {
  display.clearDisplay();
  String time = String(days) + ":" + String(hours) + ":" + String(minutes) + ":" + String(seconds);
  print_line(time, 0, 0, 2);
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

  if (alarm_enabled) {
    for (int i = 0; i < n_alarms; i++) {
      if (alarm_hours[i] == hours && alarm_minutes[i] == minutes && !alarm_triggered[i]) {
        alarm_triggered[i] = true;
        ring_alarm();
      }
    }
  }
}