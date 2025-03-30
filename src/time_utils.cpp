//This file handles the time-related functions for the ESP32 project
//including getting the current time, updating the time, and handling alarms.

#include "time_utils.h"
#include "menu_utils.h"
#include "config.h"

void print_time_now() {                                                      // Function to print the current time on the display
  display.clearDisplay();
  String time = "Day: " + String(days) + "\n" + String(hours) + ":" + String(minutes) + ":" + String(seconds);         // Format the time string
  homeScreen = true;                                                         // Set the home screen flag to true so when the time is printed, the temperature and humidity are also displayed                    
  print_line(time, 0, 0, 2);                                                 // Print the current time
  homeScreen = false;                                                        // Set the home screen flag to false 
}

void update_time_wifi() {                                                    // Function to update the time using WiFi
  struct tm timeinfo;                                                        // Create a struct to hold the time information
  getLocalTime(&timeinfo);                                                   // Get the local time
  days = timeinfo.tm_mday;                                                   // Get the day, hour, minute and second
  hours = timeinfo.tm_hour;
  minutes = timeinfo.tm_min;
  seconds = timeinfo.tm_sec;
}

void update_time_with_check_alarm() {                                        // Function to update the time and check for alarms
  update_time_wifi();                                                        // Update the time using WiFi
  print_time_now();                                                          // Print the current time

  for (int i = 0; i < n_alarms; i++) {                                       // Loop through all alarms
    if (alarm_triggered[i] && alarm_triggered_minutes[i] < minutes) {        //if the alarm was triggered and the minutes are greater than the alarm minutes, reset the alarm triggered flag so that the alarm can ring again the next day
      alarm_triggered[i] = false;
    }

    if (alarm_enabled[i]) {                                                  // Check if the alarm is enabled check if it should ring 
      if (alarm_hours[i] == hours && alarm_minutes[i] == minutes && !alarm_triggered[i]) {
        alarm_triggered[i] = true;                                           // Set the alarm triggered flag to true so that the alarm does not ring again when stopped
        alarm_triggered_minutes[i] = minutes;                                // Store the current minutes when the alarm is triggered
        ring_alarm(i);                                                       // Call the ring_alarm function to ring the alarm
      }
    }
  }
}

void ring_alarm(int alarm){                                                  // Function to ring the alarm
  bool break_happend = false;                                                // Initialize a flag to break the loop when the alarm is stopped
  display.clearDisplay();
  print_line("Alarm " + String(alarm+1),0,0,1);
  print_line("MEDICINE TIME",20,25,1);                                       // Print the alarm message
  print_line("Press ok",0,40,1); 



  digitalWrite(LED_ALARM, HIGH);                                             // Turn on the alarm LED

  while (!break_happend)                                                     // Loop until the alarm is stopped or snoozed
  {
    for(int i = 0; i < n_notes; i++){
      if(digitalRead(PB_OK) == LOW){                                        // If the OK button is pressed, turn off the alarm and break the loop
        delay(200);
        break_happend = true;
        break;
      }
      tone(BUZZER_PIN, notes[i]);                                           // Play the alarm sound using the buzzer until the ok button is pressed
      delay(500);
      noTone(BUZZER_PIN);
      delay(2);
    }
  }
  
  digitalWrite(LED_ALARM, LOW);                                            // Turn off the alarm LED
 
  goto_stopping_menu(alarm);                                               // Call the goto_stopping_menu function to stop or snooze the alarm 
  
  
}