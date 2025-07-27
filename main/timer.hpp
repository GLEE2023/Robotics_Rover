#ifndef TIMER_H
#define TIMER_H

#include <Arduino.h>
#include "driver/gptimer.h"
#include "Scheduler.hpp"

void timerUltrasonicInit(); //timer to periodically send out Ultrasonic data
void timerUARTInit(); //timer to check for UART confirmation

void timerUARTStart(); //restart UART timer

bool IRAM_ATTR timerUltrasonicInterrupt(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx);
bool IRAM_ATTR timerUARTInterrupt(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx);


#endif
