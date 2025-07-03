#include "Scheduler.hpp"

/*Most of this code was reused from ECEN 2370 which was Embedded Systems*/

static uint32_t scheduledEvents = 0;

uint32_t getScheduledEvents(){
	return scheduledEvents;
}

void addSchedulerEvent(uint32_t event_to_schedule){
	scheduledEvents |= event_to_schedule;
}


void removeSchedulerEvent(uint32_t event_to_remove){
	scheduledEvents &= ~event_to_remove;
}
