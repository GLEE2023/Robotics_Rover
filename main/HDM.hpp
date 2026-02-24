#ifndef HDM_H
#define HDM_H

#include <Arduino.h>

/* Pins */
#define RX2 17
#define TX2 16

#define BAUD_RATE 9600

enum HDMCommand : uint8_t {
  HDM_COMMAND_POWERUP,
  HDM_COMMAND_POWERDOWN,
  HDM_COMMAND_INCREMENT_DISKS,
  HDM_COMMAND_DECREMENT_DISKS,
  HDM_COMMAND_LAUNCH_DISKS,
  HDM_COMMAND_ROTATE_BARREL,
  HDM_COMMAND_RESET,
  HDM_COMMAND_NUDGE_CC,
  HDM_COMMAND_NUDGE_CCW,
  HDM_COMMAND_NUDGE_POWER_UP,
  HDM_COMMAND_NUDGE_POWER_DOWN,
};

void HDMInit(); //Initializes UART for the HDM and Rover
void HDMSendCommand(HDMCommand command); //sends the desired command to be preformed on the HDM
void HDMStatus(); //tells whether data is successfully sent

#endif