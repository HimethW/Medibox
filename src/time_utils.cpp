#include "time_utils.h"
#include "menu_utils.h"
#include "config.h"

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

void ring_alarm(int alarm){
  bool break_happend = false;
  display.clearDisplay();
  print_line("Alarm " + String(alarm+1),0,0,1);
  print_line("MEDICINE TIME",20,25,1);



  digitalWrite(LED_ALARM, HIGH);

  while (!break_happend)
  {
    for(int i = 0; i < n_notes; i++){
      if(digitalRead(PB_OK) == LOW){
        delay(200);
        break_happend = true;
        break;
      }
      tone(BUZZER_PIN, notes[i]);
      delay(500);
      noTone(BUZZER_PIN);
      delay(2);
    }
  }
  
  digitalWrite(LED_ALARM, LOW);

  goto_stopping_menu(alarm);
  
  
  
}