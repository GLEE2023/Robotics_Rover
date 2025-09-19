#include "Scheduler.hpp"
#include "ESP_Now_Transceiver.hpp"
  //  #include <esp32-hal-timer.h>
 //test

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.printf("Setting up the initialization proceedure\n");

  #if TRANSCEIVER_BUILD == HUB_BUILD
    addSchedulerEvent(CONTROLLER_INIT_EVENT);
  #else
    addSchedulerEvent(MOTOR_INIT_EVENT);
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
  else if(events & ESP_NOW_INIT_EVENT){
    ESP_Now_TransceiverInit();
  }
  else if(events & ESP_NOW_WAIT_EVENT){
    ESP_Now_Wait();
  }
#else /*TRANSCEIVER_BUILD == ROVER_BUILD */
  if(events & MOTOR_INIT_EVENT){
    ESP_Now_MotorInit();
  }
  else if(events & HDM_INIT_EVENT){
    ESP_Now_HDMInit();
  }
  else if(events & ULTRASONIC_INIT_EVENT){
    ESP_Now_UltrasonicInit();
  }
  else if(events & ESP_NOW_INIT_EVENT){
    ESP_Now_TransceiverInit();
  }
  else if(events & ESP_NOW_WAIT_EVENT){
    ESP_Now_Wait();
  }
  else if(events & ULTRASONIC_SEND_EVENT){
    ESP_Now_GetUltrasonicData();
  }
#endif


}