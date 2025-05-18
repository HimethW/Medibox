// servo_utils.h
#ifndef SERVO_UTILS_H
#define SERVO_UTILS_H

#include <ESP32Servo.h>  // or Servo.h for Arduino

// Declare external variables
extern Servo servoMotor;
extern float I;                   // Light intensity
extern int theta_offset;
extern float gamma_factor;
extern int sampling_interval;
extern int sending_interval;
extern float temp;
extern int T_med;

// Function declarations
float calc_servo_angle();
void rotateServo();

#endif