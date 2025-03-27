#include <Arduino.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHTesp.h>
#include <WiFi.h>

#include "display_utils.h"  // Include the header file
#include "time_utils.h"     // Include the header file

#define SDA 21
#define SCL 22

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

#define TEMP_UPPERLIMIT 35
#define TEMP_LOWERLIMIT 25
#define HUMIDITY_UPPERLIMIT 40
#define HUMIDITY_LOWERLIMIT 20

#define NTP_SERVER     "pool.ntp.org"
//#define UTC_OFFSET     (5.5 * 3600)
#define UTC_OFFSET_DST 0

#define BUZZER_PIN 27
#define LED_1 26
#define PB_cancel 25

#define PB_UP 34
#define PB_OK 35
#define PB_DOWN 32

#define DHTPIN 13

float UTC_OFFSET = 5.5 * 3600;

int n_notes = 8;
int C = 262;
int D = 294;
int E = 330;
int F = 349;
int G = 392;
int A = 440;
int B = 494;
int C_H = 523;
int notes[] = {C, D, E, F, G, A, B, C_H};

int days = 0;
int hours = 0;
int minutes = 0;
int seconds = 0;

bool alarm_enabled[] = {false, false};
int n_alarms = 2;
int alarm_hours[] = {0, 1};
int alarm_minutes[] = {1, 1}; 
bool alarm_triggered[] = {false, false};

int alarm_triggered_minutes[] = {0, 0};

bool tempHigh = false;
bool tempLow = false;
bool humidityHigh = false;
bool humidityLow = false; 

unsigned long timeNow = 0;
unsigned long timeLast = 0;


int max_modes = 3;
int alarm_options = 2;
int alarm_stopping_options = 2;
String modes[] = {"1)TimeZone", "2)Alarm 1", "3)Alarm 2"};
String alarm_modes[] = {"1)Set Time", "2)Delete"};

String alarm_stopping_modes[] = {"1)Stop", "2)Snooze"};

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DHTesp dhtSensor;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  WiFi.begin("Wokwi-GUEST", "", 6);
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
  }  

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_1, OUTPUT);
  pinMode(PB_cancel, INPUT);

  pinMode(PB_UP, INPUT);
  pinMode(PB_OK, INPUT);
  pinMode(PB_DOWN, INPUT);

  dhtSensor.setup(DHTPIN, DHTesp::DHT22);
  configTime(UTC_OFFSET, UTC_OFFSET_DST, NTP_SERVER);

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)){
    Serial.println(F("SSD1306 Allocation Failed"));
    for(;;);
  }

  display.display();
  delay(2000);

  display.clearDisplay();
  display.display();
}

   

void ring_alarm(){
  //Serial.println("came here");
  //Serial.println(digitalRead(PB_cancel)==HIGH);
  bool break_happend = false;
  display.clearDisplay();
  print_line("MEDICINE TIME",0,0,2);



  digitalWrite(LED_1, HIGH);

  while (!break_happend && digitalRead(PB_cancel) == HIGH)
  {
    for(int i = 0; i < n_notes; i++){
      //Serial.println(digitalRead(PB_cancel));
      if(digitalRead(PB_cancel) == LOW){
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
  
  digitalWrite(LED_1, LOW);
  display.clearDisplay();
  
  
  
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
    else if (digitalRead(PB_cancel) == LOW){
      delay(200);
      return PB_cancel;
    }
    //update_time()
  }
}

// void set_time(){
//   int temp_hours = hours;
//   int temp_minutes = minutes;
//   int temp_seconds = seconds;
  
//   while(true){
//   display.clearDisplay();
//   print_line("Set hours: " + String(temp_hours),0,0,2);
//     if(digitalRead(PB_UP) == LOW){
//       delay(200);
//       temp_hours = (temp_hours + 1) % 24;
//     }
//     else if (digitalRead(PB_DOWN) == LOW){
//       delay(200);
//       temp_hours -= 1;
//       if(temp_hours < 0){
//         temp_hours = 23;
//       }
//     }
//     else if (digitalRead(PB_OK) == LOW){
//       delay(200);
//       hours = temp_hours;
//       break;
//     }
//     else if (digitalRead(PB_cancel) == LOW){
//       delay(200);
//       return;
//     }
//   }

//   while(true){
//   display.clearDisplay();
//   print_line("Set minutes: " + String(temp_minutes),0,0,2);
//     if(digitalRead(PB_UP) == LOW){
//       delay(200);
//       temp_minutes = (temp_minutes + 1) % 60;
//     }
//     else if (digitalRead(PB_DOWN) == LOW){
//       delay(200);
//       temp_minutes -= 1;
//       if(temp_minutes < 0){
//         temp_minutes = 59;
//       }
//     }
//     else if (digitalRead(PB_OK) == LOW){
//       delay(200);
//       minutes = temp_minutes;
//       break;
//     }
//     else if (digitalRead(PB_cancel) == LOW){
//       delay(200);
//       return;
//     }
//   }

//   while(true){
//   display.clearDisplay();
//   print_line("Set seconds: " + String(temp_seconds),0,0,2);
//     if(digitalRead(PB_UP) == LOW){
//       delay(200);
//       temp_seconds = (temp_seconds + 1) % 60;
//     }
//     else if (digitalRead(PB_DOWN) == LOW){
//       delay(200);
//       temp_seconds -= 1;
//       if(temp_seconds < 0){
//         temp_seconds = 59;
//       }
//     }
//     else if (digitalRead(PB_OK) == LOW){
//       delay(200);
//       seconds = temp_seconds;
//       break;
//     }
//     else if (digitalRead(PB_cancel) == LOW){
//       delay(200);
//       return;
//     }
//   }

//   display.clearDisplay();
//   print_line("Time is set",0,0,2);
//   delay(500);
  
// }


//changed
void set_timeZone(){
  float temp_zone = UTC_OFFSET/3600;
  
  while(true){
  display.clearDisplay();
  print_line("Set TimeZone: " + String(temp_zone),0,0,2);
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
      break;
    }
    else if (digitalRead(PB_cancel) == LOW){
      delay(200);
      return;
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
  print_line("Set hours: " + String(temp_hours),0,0,2);
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
    else if (digitalRead(PB_cancel) == LOW){
      delay(200);
      display.clearDisplay(); 
      print_line("Alarm not modified",0,0,2);
      delay(500);
      return;
    }
  }

  while(true){
  display.clearDisplay();
  print_line("Set minutes: " + String(temp_minutes),0,0,2);
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
      print_line("Alarm " + String(alarm+1)+ " set",0,0,2);
      delay(500);
      alarm_enabled[alarm] = true;
      alarm_triggered[alarm] = false;
      break;
    }
    else if (digitalRead(PB_cancel) == LOW){
      delay(200);
      display.clearDisplay(); 
      print_line("Alarm not modified",0,0,2);
      delay(500);
      return;
    }
  }

  // display.clearDisplay();
  // print_line("Alarm is set",0,0,2);
  // delay(500);
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
  // else if(mode ==3){
  //   alarm_enabled = !alarm_enabled;
  //   modes[3] = String("4 - ") + (alarm_enabled ? "Disable" : "Enable") + " Alarms";
  //   display.clearDisplay();
  //   if(alarm_enabled){
  //     print_line("Alarms enabled",0,0,2);
  //   }
  //   else{
  //     print_line("Alarms disabled",0,0,2);
  //   }
  //   delay(500);
  //   return;
  // }
}

void go_to_menu(){
  int current_mode = 0;
  while(digitalRead(PB_cancel) == HIGH){
    display.clearDisplay();
    //Serial.println("cleared");
    print_line(modes[current_mode],0,0,2,true);
    int nextMode = (current_mode + 1) % max_modes;
    print_line(modes[nextMode],0,40,2,false);
    //Serial.println("printed");
    int pressed = wait_for_button_press();
    //Serial.println("pressed");

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
      run_mode(current_mode);
      break;
    case PB_cancel:
      return;
    }
  }
   
}

void goto_alarm_menu(int alarm){
  int current_mode = 0;
  while(digitalRead(PB_cancel) == HIGH){
    display.clearDisplay();

    if(alarm_enabled[alarm]){
      print_line("Enabled: ",0,0,1);
      print_line(String(alarm_hours[alarm]) + ":" + String(alarm_minutes[alarm]),60,0,2);
      print_line("Alarm " + String(alarm+1),0,10,1);
    }else{
      print_line("Not Set",0,0,2);
    }

    print_line(alarm_modes[0],0,20,2, current_mode == 0? true : false);
    

    if(alarm_enabled[alarm]){
      print_line(alarm_modes[1],0,40,2,current_mode == 1? true : false);
    }
    int pressed = wait_for_button_press();

    switch (pressed)
    {
    case PB_DOWN:
      if(alarm_enabled[alarm]){
        current_mode = (current_mode + 1) % alarm_options;
      }
      break;
    case PB_UP:
      if(alarm_enabled[alarm]){
        current_mode -= 1;
        if(current_mode < 0){
          current_mode = alarm_options - 1;
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
        print_line("Alarm " + String(alarm+1) + " deleted",0,0,2);
        delay(500);
        current_mode = 0;
      }
      break;
    case PB_cancel:
      return;
    }
  }
}

void check_temp(){
  tempHigh = false;
  tempLow = false;
  humidityHigh = false;
  humidityLow = false; 
  TempAndHumidity data = dhtSensor.getTempAndHumidity();
  int temp = data.temperature;
  int humidity = data.humidity;
  //display.clearDisplay();
  if(temp > TEMP_UPPERLIMIT){
    tempHigh = true;
    //print_line("Temperature high",0,40,1);
    //delay(100);
  }
  if(temp < TEMP_LOWERLIMIT){
    tempLow = true;
    //print_line("Temperature low",0,40,1);
    //delay(100);
  }
  if(humidity > HUMIDITY_UPPERLIMIT){
    humidityHigh = true;
    //print_line("Humidity high",0,50,1);
    //delay(100);
  }
  if(humidity < HUMIDITY_LOWERLIMIT){
    humidityLow = true;
    //print_line("Humidity low",0,50,1);
    //delay(100);
  }

}


void loop() {
  update_time_with_check_alarm();
  if(digitalRead(PB_OK) == LOW){
    go_to_menu();
  }
  check_temp();

}
