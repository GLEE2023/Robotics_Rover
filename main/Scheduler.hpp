#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include <stdint.h>
#include "BuildConfig.hpp"

#if TRANSCEIVER_BUILD == HUB_BUILD
  #define CONTROLLER_INIT_EVENT           (1 << 0)
  #define CONTROLLER_CHECK_PAIRING_EVENT  (1 << 1)
#else
  #define MOTOR_INIT_EVENT                (1 << 0)
  #define HDM_INIT_EVENT                  (1 << 1)
  #define ULTRASONIC_INIT_EVENT           (1 << 2)
#endif

#define ESP_NOW_INIT_EVENT              (1 << 3)
#define ESP_NOW_WAIT_EVENT              (1 << 4)
#define ULTRASONIC_SEND_EVENT           (1 << 5)
#define UART_CONFIRMATION_EVENT         (1 << 6) /* Not implemented due to inaccuracy of receiving back from HDM*/

/*Most of this code was reused from ECEN 2370 which was Embedded Systems, however the macros are different*/

uint32_t getScheduledEvents();
//return the scheduled events

void addSchedulerEvent(uint32_t event_to_schedule);
//Adds event to the scheduler

void removeSchedulerEvent(uint32_t event_to_remove);
//Removes event from scheduler


#endif
