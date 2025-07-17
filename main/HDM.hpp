#ifndef HDM_H
#define HDM_H

#include <Arduino.h>

/* Pins */
#define RX2 16
#define TX2 17

#define BAUD_RATE 115200

void HDMInit(); //Initializes UART for the HDM and Rover
void HDMSendCommand(String command); //sends the desired command to be preformed on the HDM
void HDMStatus(); //tells whether data is successfully sent

#endif