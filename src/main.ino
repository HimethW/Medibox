#include <Arduino.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHTesp.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ESP32Servo.h>

#include "display_utils.h"  // Include the header file
#include "time_utils.h"     // Include the header file
#include "menu_utils.h"     // Include the header file
#include "sensor_utils.h"   // Include the header file
#include "mqtt_utils.h"     // Include the header file
#include "servo_utils.h"    // Include the header file

#include "config.h"         // Include the header file

WiFiClient espClient;
PubSubClient mqttClient(espClient);
Servo servoMotor;

float UTC_OFFSET = 5.5 * 3600;

int n_notes = 8;                                          // Number of notes in the melody
int C = 262;                                              // Frequencies of the notes in Hz
int D = 294;
int E = 330;
int F = 349;
int G = 392;
int A = 440;
int B = 494;
int C_H = 523;
int notes[] = {C, D, E, F, G, A, B, C_H};

float I = 0.0;                                        // Light intensity variable

int sampling_interval = 2;                            // Sampling interval in seconds  
int sending_interval = 120;                           // Sending interval in seconds
unsigned long data_sampling_time = millis();          // variable to track the time for sampling data
unsigned long data_sending_time = millis();           // variable to track the time for sending data
float total_lux_value = 0;
int number_of_samples = 0;

int days = 0;                                            // Current date and time variables
int hours = 0;
int minutes = 0;
int seconds = 0;

bool alarm_enabled[] = {false, false};                  // Alarm enabled flags
int n_alarms = 2;
int alarm_hours[] = {0, 1};                             // Alarm time variables
int alarm_minutes[] = {1, 1}; 
bool alarm_triggered[] = {false, false};                // Alarm triggered flags

int alarm_triggered_minutes[] = {0, 0};                 // Store the minutes when the alarm is triggered

bool tempHigh = false;                                  // Temperature and humidity status flags
bool tempLow = false;
bool humidityHigh = false;
bool humidityLow = false; 
bool homeScreen = true;                                 // Flag to indicate if the home screen is displayed

float gamma_factor = 0.75;
int theta_offset = 30;
int T_med = 30;

float temp = 0;                                         // Current temperature and humidity variables
float humidity = 0;


int max_modes = 4;                                      // Maximum number of modes in the main menu
int alarm_options = 3;                                  // Number of options in the alarm menu
int alarm_stopping_options = 2;                         // Number of options in the alarm stopping menu
String modes[] = {"1)Set TimeZone", "2)Alarm 1 view/edit", "3)Alarm 2 view/edit", "()Exit"};           // Menu options for the main menu
String alarm_modes[] = {"1)Set Time", "2)Delete", "()Exit"};                                           // Menu options for the alarm menu

String alarm_stopping_modes[] = {"1)Stop", "2)Snooze"};                                               // Menu options for the alarm stopping menu

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);                             // Create the display object
DHTesp dhtSensor;                                      // Create the DHT sensor object

void setup() {

  Serial.begin(115200);                               // Initialize serial communication for debugging

  WiFi.begin("Wokwi-GUEST", "", 6);                   // Connect to WiFi network
  while (WiFi.status() != WL_CONNECTED) {             // Wait for WiFi connection
    delay(250);
  }
  
  setupMqtt();

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  pinMode(BUZZER_PIN, OUTPUT);                       // Set the buzzer and led pins as output
  pinMode(LED_ALARM, OUTPUT);
  pinMode(LED_TEMP, OUTPUT);
  pinMode(LED_HUMIDITY, OUTPUT);


  pinMode(PB_UP, INPUT);                             // Set the button pins as input                   
  pinMode(PB_OK, INPUT);
  pinMode(PB_DOWN, INPUT);
  pinMode(LDR_PIN, INPUT);                           // Set the DHT sensor pin as input

  servoMotor.attach(SERVO_PIN);                      // Attach the servo motor to the servo pin

  dhtSensor.setup(DHTPIN, DHTesp::DHT22);           // Initialize the DHT sensor
  configTime(UTC_OFFSET, UTC_OFFSET_DST, NTP_SERVER);          // Set the timezone using the configTime function

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)){    // Initialize the display
    Serial.println(F("SSD1306 Allocation Failed"));
    for(;;);                                                   // Don't proceed, loop forever
  }

  display.display(); 
  delay(2000);

  display.clearDisplay();
  display.display();
}

float mapLinear(float value) {
  value = constrain(value, 0.1, 100916.57);                   // Constrain the value to the range of 0.1 to 100916.57 (these values were obtained from the LDR by manually measuring the max and min light intensity)
  return (value - 0.1) * (1 - 0) / (100916.57 - 0.1) + 0;     // Map the value to the range of 0 to 1
}


void loop() {
  if(!mqttClient.connected()){                                // If the MQTT client is not connected, try to connect to the broker
    connectToBroker();
  }

  mqttClient.loop();

  update_time_with_check_alarm();                             // Update the time and check for alarms
  if(digitalRead(PB_OK) == LOW){                              // If the OK button is pressed, go to the main menu
    delay(200);
    go_to_menu();
  }

  check_temp();                                               // Check the temperature and humidity using the DHT sensor

  if(millis()-data_sampling_time > sampling_interval*1000){       // If the sampling interval has passed, check the light sensor
    data_sampling_time = millis();                                // Update the sample data time
    check_light();
  }
  
  if(millis()-data_sending_time > sending_interval*1000){         // If the data sending interval has passed, send the data to the server
    data_sending_time = millis();                                 // Update the data sending time
    I = mapLinear(total_lux_value/number_of_samples);             // Calculate the average light intensity and map it between 0 and 1
    publish_data("Light_intensity",I);                            // Publish the data to the server
    total_lux_value = 0;                                          // Reset the total lux value
    number_of_samples = 0;                                        // Reset the number of samples
  }

  rotateServo();                                                  // Rotate the servo motor based on the light intensity and temperature
}
