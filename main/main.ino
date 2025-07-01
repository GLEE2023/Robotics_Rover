#include "Scheduler.hpp"
#include "ESP_Now_Transceiver.hpp"

#define HUB_INIT   0 
#define ROVER_INIT 1
#define TRANSCEIVER_INIT HUB_INIT

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.printf("Setting up the initialization proceedure\n");

  #if TRANSCEIVER_INIT == HUB_INIT
    addSchedulerEvent(CONTROLLER_INIT_EVENT);
  #else
  //
    addSchedulerEvent(ESP_NOW_INIT_EVENT);
  #endif
}

void loop() {
  // put your main code here, to run repeatedly:

  uint32_t events = getScheduledEvents();

  if(events & CONTROLLER_INIT_EVENT){
    ESP_NowControllerInit();
  }
  else if(events & CONTROLLER_CHECK_PAIRING_EVENT){
   ESP_Now_Hub_Pair_Controller();
   delay(100);
  }
  else if(events & ESP_NOW_INIT_EVENT){
    ESP_Now_Transceiver_Init();
  }
  else if(events & ESP_NOW_WAIT_EVENT){
    ESP_Now_Hub_Wait();
  }
}
