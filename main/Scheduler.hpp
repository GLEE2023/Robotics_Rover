#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#define CONTROLLER_INIT_EVENT           (1 << 0)
#define ESP_NOW_INIT_EVENT              (1 << 1)

/*Most of this code was reused from ECEN 2370 which was Embedded Systems, however the macros are different*/

uint32_t getScheduledEvents();
//return the scheduled events

void addSchedulerEvent(uint32_t event_to_schedule);
//Adds event to the scheduler

void removeSchedulerEvent(uint32_t event_to_remove);
//Removes event from scheduler


#endif
