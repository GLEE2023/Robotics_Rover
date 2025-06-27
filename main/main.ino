

#define HUB_INIT   0 
#define ROVER_INIT 1
#define TRANSCEIVER_INIT HUB_INIT

#if TRANSCEIVER_INIT == HUB_INIT
  #include "ESP_Now_Transceive_Hub.hpp"
#else
  #include "ESP_Now_Transceive_Rover.hpp"
#endif

void setup() {
  // put your setup code here, to run once:
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
