#ifndef MOTOR_H
#define MOTOR_H

#include "Arduino.h"

#define MOTOR_ONE_PWM_PIN   22
#define MOTOR_TWO_PWM_PIN   23
#define MOTOR_THREE_PWM_PIN 33
#define MOTOR_FOUR_PWM_PIN  32

#define MOTOR_ONE_ENC_PIN   18
#define MOTOR_TWO_ENC_PIN   19
#define MOTOR_THREE_ENC_PIN 25
#define MOTOR_FOUR_ENC_PIN  26

#define MOTOR_LEFT_DIR_PIN   21
#define MOTOR_RIGHT_DIR_PIN  27


#define LEFT_SIDE            0
#define RIGHT_SIDE           1

#define MOTOR_COUNT          4
#define PULSES_PER_REVOLUTION 24
#define MICROS_PER_SECOND    1e6
#define SECONDS_PER_MINUTE   60
#define WHEEL_DIAMETER       5.5 /* this is in inches */
#define GEAR_RATIO           189 /* Note encoder operates before the gear ratio takes effect*/
#define MAX_WHEEL_RPM        45
#define MAX_ENC_RPM          (MAX_WHEEL_RPM * GEAR_RATIO)
#define WHEEL_CIRCUMFERENCE  (PI * WHEEL_DIAMETER)
#define MAX_VOLTAGE          3.3
#define RPM_STEP_SIZE        MAX_WHEEL_RPM / 5 /*Want 5 increments of speed*/

#define FORWARDS             0
#define BACKWARDS            1 
#define INCREASE_SPEED             1
#define DECREASE_SPEED            -1

#define RPM_FROM_FREQ(frequency)          ( (frequency * SECONDS_PER_MINUTE) / PULSES_PER_REVOLUTION )
#define VELOCITY_FROM_RPM(rpm)            ( (WHEEL_CIRCUMFERENCE * rpm) / SECONDS_PER_MINUTE ) /* returns velocity in inches/second */

#define MOTOR_ONE            0
#define MOTOR_TWO            1
#define MOTOR_THREE          2
#define MOTOR_FOUR           3


void motorInit(); //Initializes motors

void updateDesiredRPM(int change); //Updates the global desired speed for each motor (this happens through the controller from ESP-NOW), the user will be inputting a desired RPM, which the interface displays as a velocity but that would be converted in code
void matchDesiredRPM(); //Updates each motors speed to be the global desired speed, can incorporate a PID in the future

uint32_t calculateRPM(uint32_t motorNum); //finds the RPM and returns it
uint32_t getPulsePeriod(uint32_t motorNum); //Disables interrupts and returns the period, this is a safer way to get the pulses rather than just reading from the global variable

void motorDriveLeft(uint8_t dir);
void motorDriveRight(uint8_t dir);
void motorDrive(uint8_t side, uint8_t dir);

void IRAM_ATTR motorOneEncoderISR();
void IRAM_ATTR motorTwoEncoderISR();
void IRAM_ATTR motorThreeEncoderISR();
void IRAM_ATTR motorFourEncoderISR();

#endif