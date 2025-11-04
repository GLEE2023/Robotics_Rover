#ifndef MOTOR_H
#define MOTOR_H

#include "Arduino.h"

#define MOTOR_FR_PWM_PIN   33
#define MOTOR_BR_PWM_PIN   32
#define MOTOR_FL_PWM_PIN   22
#define MOTOR_BL_PWM_PIN   23

#define MOTOR_FR_ENC_PIN   25
#define MOTOR_BR_ENC_PIN   26
#define MOTOR_FL_ENC_PIN   19
#define MOTOR_BL_ENC_PIN   18

#define MOTOR_LEFT_DIR_PIN   21
#define MOTOR_RIGHT_DIR_PIN  27


#define LEFT_SIDE            1
#define RIGHT_SIDE           0

#define MOTOR_COUNT          4
#define PULSES_PER_REVOLUTION 12
#define MICROS_PER_SECOND    1e6
#define SECONDS_PER_MINUTE   60
#define WHEEL_DIAMETER       5.5 /* this is in inches */
#define GEAR_RATIO           189 /* Note encoder operates before the gear ratio takes effect*/
#define MAX_WHEEL_RPM        45
#define MAX_ENC_RPM          (MAX_WHEEL_RPM * GEAR_RATIO)
#define WHEEL_CIRCUMFERENCE  (PI * WHEEL_DIAMETER)
#define MAX_VOLTAGE          3.3f
#define RPM_STEP_SIZE        MAX_WHEEL_RPM / 5 /*Want 5 increments of speed*/

#define FORWARDS             0
#define BACKWARDS            1 
#define INCREASE_SPEED             1
#define DECREASE_SPEED            -1

#define RPM_FROM_FREQ(frequency)          ( (frequency * SECONDS_PER_MINUTE) / PULSES_PER_REVOLUTION )
#define VELOCITY_FROM_RPM(rpm)            ( (WHEEL_CIRCUMFERENCE * rpm) / SECONDS_PER_MINUTE ) /* returns velocity in inches/second */
#define WHEEL_RPM_FROM_ENC_RPM(enc_rpm)   ( (enc_rpm) / GEAR_RATIO )


#define MOTOR_FR            0
#define MOTOR_BR            1
#define MOTOR_FL            2
#define MOTOR_BL            3


void motorInit(); //Initializes motors

int getDesiredRPM(uint8_t index); //gets the correct RPM for either side
/* Wrapper functions for updateDesiredRPM */
void updateDesiredRPMLeft(int change);
void updateDesiredRPMRight(int change);
/*                           */
void updateDesiredRPM(uint8_t side, int change); //Updates the global desired speed for each motor (this happens through the controller from ESP-NOW), the user will be inputting a desired RPM, which the interface displays as a velocity but that would be converted in code
void matchDesiredRPM(); //Updates each motors speed to be the global desired speed, can incorporate a PID in the future

void rampDown(uint8_t side); //adjusts speed to 0

void setDesiredRPM(uint8_t side, int rpm);

uint32_t calculateRPM(uint32_t motorNum); //finds the RPM and returns it
uint32_t getPulsePeriod(uint32_t motorNum); //Disables interrupts and returns the period, this is a safer way to get the pulses rather than just reading from the global variable

void motorDriveLeft(uint8_t dir);
void motorDriveRight(uint8_t dir);
void motorDrive(uint8_t side, uint8_t dir);

void IRAM_ATTR motorFREncoderISR();
void IRAM_ATTR motorBREncoderISR();
void IRAM_ATTR motorFLEncoderISR();
void IRAM_ATTR motorBLEncoderISR();

#endif