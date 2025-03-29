//This file handles all menu navigation functions and alarm settings

#include "menu_utils.h"

void go_to_menu(){
  int current_mode = 0;                                             // Initialize the current menu mode to 0
  while(true){
    display.clearDisplay();                                         // Clear the display                        
  
    print_line(modes[0],0,0,1,current_mode == 0? true : false);     // Print the menu options and highlight it if selected
  
    print_line(modes[1],0,15,1,current_mode == 1? true : false);
    print_line(modes[2],0,30,1,current_mode == 2? true : false);
    print_line(modes[3],0,45,1,current_mode == 3? true : false);
  
    int pressed = wait_for_button_press();                          // Wait for button press                
  
  
    switch (pressed)                                                // Handle button press events
    {
      case PB_DOWN:
        current_mode = (current_mode + 1) % max_modes;              // Increment the current mode and wrap around if it exceeds the maximum modes
        break;
      case PB_UP:
        current_mode -= 1;
        if(current_mode < 0){
          current_mode = max_modes - 1;                            // Decrement the current mode and wrap around if it goes below 0
        }
        break;
      case PB_OK:
        if(current_mode == 3){                                    // If the current mode is 3, go to the home screen
          return;
        }
        run_mode(current_mode);                                   // Call the run_mode function with the current mode
        break;
    }
  }
     
}

void goto_alarm_menu(int alarm){                                  // Function to navigate to the alarm menu
  int current_mode = 0;                                           // Initialize the current mode to 0
  while(true){
    display.clearDisplay();
  
    if(alarm_enabled[alarm]){
      print_line("Enabled: ",0,0,1);                              // Print the alarm status
      print_line(String(alarm_hours[alarm]) + ":" + String(alarm_minutes[alarm]),60,0,2);       // Print the alarm time
      print_line("Alarm " + String(alarm+1),0,10,1);              // Print the alarm number
    }else{
      print_line("Not Set",0,0,2);                                // Print "Not Set" if the alarm is not enabled
    }
  
    print_line(alarm_modes[0],0,30,1, current_mode == 0? true : false);                         // Print the set alarm option and highlight it if selected
      
  
    if(alarm_enabled[alarm]){                                    // If the alarm is enabled, print the delete and exit options
      print_line(alarm_modes[1],0,40,1,current_mode == 1? true : false);
      print_line(alarm_modes[2],0,50,1,current_mode == 2? true : false);
    }else{                                                       // If the alarm is not enabled, print the exit option only
      print_line(alarm_modes[2],0,40,1,current_mode == 2? true : false);
    }
    int pressed = wait_for_button_press();
  
    switch (pressed)
    {
      case PB_DOWN:
        current_mode = (current_mode + 1) % alarm_options;      // Increment the current mode and wrap around if it exceeds the maximum alarm options
        if(!alarm_enabled[alarm]){
          if(current_mode == 1){
            current_mode = 2;
          }
        }
        break;
      case PB_UP:
        current_mode -= 1;
        if(current_mode < 0){
          current_mode = alarm_options - 1;                    // Decrement the current mode and wrap around if it goes below 0
        }
        if(!alarm_enabled[alarm]){
          if(current_mode == 1){
            current_mode = 0;
          }
        }
        break;
      case PB_OK:
        if(current_mode == 0){
          set_alarm(alarm);                                   // Call the set_alarm function to set the alarm time
        }
        else if(current_mode == 1){                           // If the current mode is delete, then delete the alarm
          alarm_enabled[alarm] = false;
          display.clearDisplay();
          print_line("Alarm " + String(alarm+1) + "\n deleted" ,20,15,2);
          delay(1000);
          current_mode = 0;
        }
        else if(current_mode == 2){                           // If the current mode is exit, then return to the previous menu
          return;
        }
        break;
    }
  }
}

void goto_stopping_menu(int alarm){                          // Function to navigate to the alarm stopping menu
  int current_mode = 0;                                      // Initialize the current mode to 0(stop alarm)
  while(true){
    display.clearDisplay();
    print_line(alarm_stopping_modes[0],0,0,1,current_mode == 0? true : false);                // Print the stop alarm option and highlight it if selected     
    print_line(alarm_stopping_modes[1],0,10,1,current_mode == 1? true : false);               // Print the snooze option and highlight it if selected
    int pressed = wait_for_button_press();
    switch (pressed)                                         // Handle button press events by selecting the option or moving up and down
    {
      case PB_DOWN:
        current_mode = (current_mode + 1) % alarm_stopping_options;
        break;
      case PB_UP:
        current_mode -= 1;
        if(current_mode < 0){
          current_mode = alarm_stopping_options - 1;
        }
        break;
      case PB_OK:
        if(current_mode == 0){
          display.clearDisplay();
          print_line("Alarm " + String(alarm+1) + "\n Stopped",20,15,2);                       // Print the alarm stopped message
          delay(1000);
          return;
        }else if(current_mode == 1){
          display.clearDisplay();
          print_line("Alarm " + String(alarm+1) + "\n    Snoozed for " + String(SNOOZE_TIME_MINUTES) + "\n       minutes",40,10,1);  
          delay(1000);
          alarm_minutes[alarm] += SNOOZE_TIME_MINUTES;                                        // Add the snooze time to the current alarm timr
          if(alarm_minutes[alarm] >= 60){
            alarm_minutes[alarm] %= 60;                      // Wrap around the minutes if it exceeds 60
            alarm_hours[alarm] += 1;   
            if(alarm_hours[alarm] == 24){                   // Wrap around the hours if it exceeds 24
              alarm_hours[alarm] = 0;
            }
          
          }
          return;                                           // Return to the previous menu after snoozing the alarm
        }
    }
  }
}

void run_mode(int mode){                                    // Function to run the selected mode
  if(mode == 0){
    set_timeZone();                                         // Call the set_timeZone function to set the timezone
    return;
  }
  else if(mode == 1 || mode ==2){
    goto_alarm_menu(mode-1);                                // Call the goto_alarm_menu function to set or delete the alarm
    return;
  }
  
}

void set_timeZone(){                                        // Function to set the timezone
  float temp_zone = UTC_OFFSET/3600;                        // Convert the UTC offset to hours
    
  while(true){
  display.clearDisplay();
  print_line("Set TimeZone: \n" + String(temp_zone) + "\n\n\nPress ok to set",0,0,1);
    if(digitalRead(PB_UP) == LOW){                          // If the up button is pressed, increment the timezone by 0.25 hours(15 minutes)
      delay(200);
      temp_zone += 0.25;
      if(temp_zone > 14){                                   // If the timezone exceeds 14 hours, set it to 14 as this is the upper limit
        temp_zone = 14;
      }
    }
    else if (digitalRead(PB_DOWN) == LOW){ 
      delay(200);
      temp_zone -= 0.25;
      if(temp_zone < -12){                                 // If the timezone goes below -12 hours, set it to -12 as this is the lower limit
        temp_zone = -12;
      }
    }
    else if (digitalRead(PB_OK) == LOW){
      delay(200);
      UTC_OFFSET = temp_zone * 3600;                      // Convert the timezone back to seconds
      configTime(UTC_OFFSET, UTC_OFFSET_DST, NTP_SERVER); // Set the timezone using the configTime function
      display.clearDisplay();
      print_line("TimeZone\n    set",15,15,2);
      delay(1000);
      break;
    }
  }
}
  
void set_alarm(int alarm){
  int temp_hours = 0;
  int temp_minutes = 0;

  if(alarm_enabled[alarm]){                             // If the alarm is already set, get the current alarm time
    temp_hours = alarm_hours[alarm];                    
    temp_minutes = alarm_minutes[alarm];
  }else{                                               // If the alarm is not set, get the current time
    temp_hours = hours;
    temp_minutes = minutes;
  }
    
  while(true){
  display.clearDisplay();
  print_line("Set hours: \n" + String(temp_hours) + "\n\n\nPress ok to set",0,0,1);                 // Print the current hours and wait for button press to set hours
    if(digitalRead(PB_UP) == LOW){
      delay(200);
      temp_hours = (temp_hours + 1) % 24;
    }
    else if (digitalRead(PB_DOWN) == LOW){
      delay(200);
      temp_hours -= 1;
      if(temp_hours < 0){
        temp_hours = 23;
      }
    }
    else if (digitalRead(PB_OK) == LOW){              // If the ok button is pressed, set the alarm hours and break the loop
      delay(200);
      alarm_hours[alarm] = temp_hours;
      break;
    }

  }

  while(true){
  display.clearDisplay();
  print_line("Set minutes: \n" + String(temp_minutes) + "\n\n\nPress ok to set",0,0,1);         // Print the current minutes and wait for button press to set minutes
    if(digitalRead(PB_UP) == LOW){
      delay(200);
      temp_minutes = (temp_minutes + 1) % 60;
    }
    else if (digitalRead(PB_DOWN) == LOW){
      delay(200);
      temp_minutes -= 1;
      if(temp_minutes < 0){
        temp_minutes = 59;
      }
    }
    else if (digitalRead(PB_OK) == LOW){             // If the ok button is pressed, set the alarm minutes and break the loop
      delay(200);
      alarm_minutes[alarm] = temp_minutes;
      display.clearDisplay(); 
      print_line("Alarm " + String(alarm+1)+ "\n   set",20,15,2);
      delay(1000);
      alarm_enabled[alarm] = true;                  // Set the alarm enabled flag to true
      alarm_triggered[alarm] = false;               // Set the alarm triggered flag to false in order for the alarm to ring
      break;
    }
    
  }
  
}

int wait_for_button_press(){                        // Function to wait for button press
  int pressed = 0;
  while(true){
    if(digitalRead(PB_UP) == LOW){                  // If the up button is pressed, return PB_UP
      delay(200);
      return PB_UP;
    }
    else if (digitalRead(PB_DOWN) == LOW){          // If the down button is pressed, return PB_DOWN
      delay(200);
      return PB_DOWN;
    }
    else if (digitalRead(PB_OK) == LOW){           // If the ok button is pressed, return PB_OK
      delay(200);
      return PB_OK;
    }
  }
}