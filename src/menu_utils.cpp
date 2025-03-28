#include "menu_utils.h"

void go_to_menu(){
  int current_mode = 0;
  while(true){
    display.clearDisplay();
  
    print_line(modes[0],0,0,1,current_mode == 0? true : false);
  
    print_line(modes[1],0,15,1,current_mode == 1? true : false);
    print_line(modes[2],0,30,1,current_mode == 2? true : false);
    print_line(modes[3],0,45,1,current_mode == 3? true : false);
  
    int pressed = wait_for_button_press();
  
  
    switch (pressed)
    {
      case PB_DOWN:
        current_mode = (current_mode + 1) % max_modes;
        break;
      case PB_UP:
        current_mode -= 1;
        if(current_mode < 0){
          current_mode = max_modes - 1;
        }
        break;
      case PB_OK:
        if(current_mode == 3){
          return;
        }
        run_mode(current_mode);
        break;
    }
  }
     
}

void goto_alarm_menu(int alarm){
  int current_mode = 0;
  while(true){
    display.clearDisplay();
  
    if(alarm_enabled[alarm]){
      print_line("Enabled: ",0,0,1);
      print_line(String(alarm_hours[alarm]) + ":" + String(alarm_minutes[alarm]),60,0,2);
      print_line("Alarm " + String(alarm+1),0,10,1);
    }else{
      print_line("Not Set",0,0,2);
    }
  
    print_line(alarm_modes[0],0,30,1, current_mode == 0? true : false);
      
  
    if(alarm_enabled[alarm]){
      print_line(alarm_modes[1],0,40,1,current_mode == 1? true : false);
      print_line(alarm_modes[2],0,50,1,current_mode == 2? true : false);
    }else{
      print_line(alarm_modes[2],0,40,1,current_mode == 2? true : false);
    }
    int pressed = wait_for_button_press();
  
    switch (pressed)
    {
      case PB_DOWN:
        current_mode = (current_mode + 1) % alarm_options;
        if(!alarm_enabled[alarm]){
          if(current_mode == 1){
            current_mode = 2;
          }
        }
        break;
      case PB_UP:
        current_mode -= 1;
        if(current_mode < 0){
          current_mode = alarm_options - 1;
        }
        if(!alarm_enabled[alarm]){
          if(current_mode == 1){
            current_mode = 0;
          }
        }
        break;
      case PB_OK:
        if(current_mode == 0){
          set_alarm(alarm);
        }
        else if(current_mode == 1){
          alarm_enabled[alarm] = false;
          display.clearDisplay();
          print_line("Alarm " + String(alarm+1) + "\n deleted" ,20,15,2);
          delay(1000);
          current_mode = 0;
        }
        else if(current_mode == 2){
          return;
        }
        break;
    }
  }
}

void goto_stopping_menu(int alarm){
  int current_mode = 0;
  while(true){
    display.clearDisplay();
    print_line(alarm_stopping_modes[0],0,0,1,current_mode == 0? true : false);
    print_line(alarm_stopping_modes[1],0,10,1,current_mode == 1? true : false);
    int pressed = wait_for_button_press();
    switch (pressed)
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
          print_line("Alarm " + String(alarm+1) + "\n Stopped",20,15,2);
          delay(1000);
          return;
        }else if(current_mode == 1){
          display.clearDisplay();
          print_line("Alarm " + String(alarm+1) + "\n    Snoozed for " + String(SNOOZE_TIME_MINUTES) + "\n       minutes",40,10,1);  
          delay(1000);
          alarm_minutes[alarm] += SNOOZE_TIME_MINUTES;
          if(alarm_minutes[alarm] >= 60){
            alarm_minutes[alarm] %= 60;
            alarm_hours[alarm] += 1;
            if(alarm_hours[alarm] == 24){
              alarm_hours[alarm] = 0;
            }
          
          }
          return;
        }
    }
  }
}

void run_mode(int mode){
  if(mode == 0){
    set_timeZone();
    return;
  }
  else if(mode == 1 || mode ==2){
    goto_alarm_menu(mode-1);
    return;
  }
  
}

void set_timeZone(){
  float temp_zone = UTC_OFFSET/3600;
    
  while(true){
  display.clearDisplay();
  print_line("Set TimeZone: \n" + String(temp_zone) + "\n\n\nPress ok to set",0,0,1);
    if(digitalRead(PB_UP) == LOW){
      delay(200);
      temp_zone += 0.25;
      if(temp_zone > 14){
        temp_zone = 14;
      }
    }
    else if (digitalRead(PB_DOWN) == LOW){
      delay(200);
      temp_zone -= 0.25;
      if(temp_zone < -12){
        temp_zone = -12;
      }
    }
    else if (digitalRead(PB_OK) == LOW){
      delay(200);
      UTC_OFFSET = temp_zone * 3600;
      configTime(UTC_OFFSET, UTC_OFFSET_DST, NTP_SERVER);
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

  if(alarm_enabled[alarm]){
    temp_hours = alarm_hours[alarm];
    temp_minutes = alarm_minutes[alarm];
  }else{
    temp_hours = hours;
    temp_minutes = minutes;
  }
    
  while(true){
  display.clearDisplay();
  print_line("Set hours: \n" + String(temp_hours) + "\n\n\nPress ok to set",0,0,1);
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
    else if (digitalRead(PB_OK) == LOW){
      delay(200);
      alarm_hours[alarm] = temp_hours;
      break;
    }

  }

  while(true){
  display.clearDisplay();
  print_line("Set minutes: \n" + String(temp_minutes) + "\n\n\nPress ok to set",0,0,1);
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
    else if (digitalRead(PB_OK) == LOW){
      delay(200);
      alarm_minutes[alarm] = temp_minutes;
      display.clearDisplay(); 
      print_line("Alarm " + String(alarm+1)+ "\n   set",20,15,2);
      delay(1000);
      alarm_enabled[alarm] = true;
      alarm_triggered[alarm] = false;
      break;
    }
    
  }
  
}

int wait_for_button_press(){
  int pressed = 0;
  while(true){
    if(digitalRead(PB_UP) == LOW){
      delay(200);
      return PB_UP;
    }
    else if (digitalRead(PB_DOWN) == LOW){
      delay(200);
      return PB_DOWN;
    }
    else if (digitalRead(PB_OK) == LOW){
      delay(200);
      return PB_OK;
    }
  }
}