#include "servo_utils.h"
#include "mqtt_utils.h"  
#include <Arduino.h>

float calc_servo_angle(){
  float ratio_intervals = (float)sampling_interval / sending_interval;     // Calculate the angle of the servo motor based on the light intensity and temperature
  float log_term = log(ratio_intervals);
  float temp_ratio = temp / T_med;
  float scaling = (180.0 - theta_offset) * I * gamma_factor;
  float theta = theta_offset + scaling * log_term * temp_ratio;
  
  theta = constrain(round(theta * 10.0) / 10.0, 0.0, 180.0);               // Constrain the angle to be between 0 and 180 degrees and round it to one decimal place

  return theta; 
}

void rotateServo(){
  float servo_angle = calc_servo_angle();                                  // Calculate the servo angle
  publish_data("servo_angle", servo_angle);                                // Publish the servo angle to the server
  servoMotor.write(servo_angle);                                           // Write the angle to the servo motor

}