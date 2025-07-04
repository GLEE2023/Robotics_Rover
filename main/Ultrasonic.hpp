#ifndef ULTRASONIC_H
#define ULTRASONIC_H

typedef struct ultrasonic_sensor_t{
  uint8_t  trigPinNumber = ;
  uint8_t  echoPinNumber;
  uint16_t distance;
} ultrasonic_sensor_t;

#endif