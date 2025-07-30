#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <stdint.h>
// #if TRANSCEIVER_BUILD == ROVER_BUILD 
// #include "timer.hpp"
// #endif

#define SOUND_SPEED 0.0135039 // inch/uS
#define ULTRASONIC_COUNT     4

#define TRIG_PIN             13
#define ECHO_PIN_ZERO        35
#define ECHO_PIN_ONE         34
#define ECHO_PIN_TWO         39
#define ECHO_PIN_THREE       36

void ultrasonicInit();

void updateUltrasonicSensors(); //Sends out a trig pulse to update distances for all sensors

void getUltrasonic(float distances[]); //stores the distances calculated into the input array 


#endif