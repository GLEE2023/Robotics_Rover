#include "Scheduler.hpp"
#include "ESP_Now_Transceiver.hpp"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.printf("Setting up the initialization proceedure\n");

  #if TRANSCEIVER_BUILD == HUB_BUILD
    addSchedulerEvent(CONTROLLER_INIT_EVENT);
  #else
  //
    addSchedulerEvent(ESP_NOW_INIT_EVENT);
  #endif
}

void loop() {
  // put your main code here, to run repeatedly:

  uint32_t events = getScheduledEvents();

#if TRANSCEIVER_BUILD == HUB_BUILD
  if(events & CONTROLLER_INIT_EVENT){
    ESP_Now_ControllerInit();
  }
  else if(events & CONTROLLER_CHECK_PAIRING_EVENT){
   ESP_Now_PairController();
   delay(100);
  }
  else /* Else required so we don't trigger two events in the same loop */ 
#endif
  if(events & ESP_NOW_INIT_EVENT){
    ESP_Now_TransceiverInit();
  }
  else if(events & ESP_NOW_WAIT_EVENT){
    ESP_Now_Wait();
  }
}
