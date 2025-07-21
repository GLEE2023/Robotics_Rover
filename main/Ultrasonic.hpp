#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <stdint.h>

#define SOUND_SPEED 0.0135039 // inch/uS
#define ULTRASONIC_COUNT     4

#define TRIG_PIN             13
#define ECHO_PIN_ZERO        35
#define ECHO_PIN_ONE         34
#define ECHO_PIN_TWO         39
#define ECHO_PIN_THREE       36

void ultrasonicInit();

void updateUltrasonicSensors();

void getUltrasonic(float distances[]); //returns the data 

#endif