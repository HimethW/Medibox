#include <Arduino.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHTesp.h>
#include <WiFi.h>

#include "display_utils.h"  // Include the header file
#include "time_utils.h"     // Include the header file
#include "menu_utils.h"     // Include the header file
#include "sensor_utils.h"   // Include the header file

#include "config.h"        // Include the header file

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
bool homeScreen = true;

unsigned long timeNow = 0;
unsigned long timeLast = 0;

float temp = 0;
float humidity = 0;


int max_modes = 4;
int alarm_options = 3;
int alarm_stopping_options = 2;
String modes[] = {"1)Set TimeZone", "2)Alarm 1 view/edit", "3)Alarm 2 view/edit", "()Exit"};
String alarm_modes[] = {"1)Set Time", "2)Delete", "()Exit"};

String alarm_stopping_modes[] = {"1)Stop", "2)Snooze"};

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DHTesp dhtSensor;

void setup() {

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
  pinMode(LED_ALARM, OUTPUT);
  pinMode(LED_TEMP, OUTPUT);
  pinMode(LED_HUMIDITY, OUTPUT);


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

   





void loop() {
  update_time_with_check_alarm();
  if(digitalRead(PB_OK) == LOW){
    delay(200);
    go_to_menu();
  }
  check_temp();

}
