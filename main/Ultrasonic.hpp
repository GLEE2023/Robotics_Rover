#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <stdint.h>

#define ULTRASONIC_ONE_ECHO       15
#define ULTRASONIC_TWO_ECHO       15
#define ULTRASONIC_THREE_ECHO     15
#define ULTRASONIC_FOUR_ECHO      15


typedef struct ultrasonic_sensor_t{
  uint8_t  trigPinNumber;
  uint8_t  echoPinNumber;
  uint16_t distance;
} ultrasonic_sensor_t;

void ultrasonicInit();
void calculateDistance(uint8_t sensorNumber);
void getUltrasonic();

#endif