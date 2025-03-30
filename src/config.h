//This file contains the configuration settings for the ESP32 project.

#ifndef CONFIG_H
#define CONFIG_H

#define SDA 21
#define SCL 22
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

#define TEMP_UPPERLIMIT 32.0
#define TEMP_LOWERLIMIT 24.0
#define HUMIDITY_UPPERLIMIT 80.0
#define HUMIDITY_LOWERLIMIT 65.0
#define SNOOZE_TIME_MINUTES 5

#define NTP_SERVER "pool.ntp.org"
#define UTC_OFFSET_DST 0

#define BUZZER_PIN 27
#define LED_ALARM 26
#define LED_TEMP 19
#define LED_HUMIDITY 18
#define PB_UP 34
#define PB_OK 35
#define PB_DOWN 32
#define DHTPIN 13

#endif