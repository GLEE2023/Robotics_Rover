#ifndef MOTOR_H
#define MOTOR_H

#include "Arduino.h"

#define MOTOR_ONE_PWM_PIN   21
#define MOTOR_TWO_PWM_PIN   19
#define MOTOR_THREE_PWM_PIN 18
#define MOTOR_FOUR_PWM_PIN  5

#define MOTOR_ONE_ENC_PIN   32
#define MOTOR_TWO_ENC_PIN   33
#define MOTOR_THREE_ENC_PIN 23
#define MOTOR_FOUR_ENC_PIN  25

#define MOTOR_LEFT_DIR_PIN   22
#define MOTOR_RIGHT_DIR_PIN  23

#define MOTOR_COUNT          4
#define PULSE_PER_REVOLUTION 24
#define MICROS_PER_SECOND    1000000
#define SECONDS_PER_MINUTE   60
#define WHEEL_DIAMETER       5.5 /* this is in inches */
#define WHEEL_CIRCUMFERENCE  (PI * WHEEL_DIAMETER)
#define MAX_VOLTAGE          3.3

#define RPM_FROM_FREQ(frequency)          ( (frequency * SECONDS_PER_MINUTE) / MICROS_PER_SECOND )
#define VELOCITY_FROM_RPM(rpm)            ( (WHEEL_CIRCUMFERENCE * rpm) / SECONDS_PER_MINUTE ) /* returns velocity in inches/second */

#define MOTOR_ONE            0
#define MOTOR_TWO            1
#define MOTOR_THREE          2
#define MOTOR_FOUR           3


volatile float desiredRPM;

const uint8_t motorPWMPIN[MOTOR_COUNT] = {MOTOR_ONE_PWM_PIN, MOTOR_TWO_PWM_PIN, MOTOR_THREE_PWM_PIN, MOTOR_FOUR_PWM_PIN};
volatile uint32_t motorPulsePeriod[MOTOR_COUNT] = {0}; //Time between last encoder pulse and current encoder plse
volatile uint32_t motorLastPulseTime[MOTOR_COUNT] = {0}; 

float motorActualRPM[MOTOR_COUNT] = {0}; //Actual RPM calculated from the encoder pulses
float motorOutputVoltage[MOTOR_COUNT] = {0}; //Outputted PWM to get the desired RPM


void motorInit(); //Initializes motors

void updateDesiredRPM(float newRPM); //Updates the global desired speed for each motor (this happens through the controller from ESP-NOW), the user will be inputting a desired RPM, which the interface displays as a velocity but that would be converted in code

void matchDesiredRPM(); //Updates each motors speed to be the global desired speed, can incorporate a PID in the future

uint32_t calculateRPM(uint32_t motorNum); //finds the RPM and returns it

uint32_t getPulsePeriod(uint32_t motorNum); //Disables interrupts and returns the period, this is a safer way to get the pulses rather than just reading from the global variable

void IRAM_ATTR motorOneEncoderISR();
void IRAM_ATTR motorTwoEncoderISR();
void IRAM_ATTR motorThreeEncoderISR();
void IRAM_ATTR motorFourEncoderISR();

#endif