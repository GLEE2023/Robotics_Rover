#include "ESP_Now_Transceiver.hpp"
#include "Scheduler.hpp"

#define PEER_ADDRESS_SIZE 6 //numbers of elements which is 6 bytes

#define DATA_TRANSMIT_TYPE_CONTROLLER 0
#define DATA_TRANSMIT_TYPE_ULTRASONIC 1

#define SANTIZATION_DEADZONE          10
#define SANTIZATION_CHANGED(new, old)     (abs((new)-(old)) > SANTIZATION_DEADZONE) //macro to detect if there is a significant change
#define CLEAR_COUNT(count)            (count = 0)

#if TRANSCEIVER_BUILD == HUB_BUILD //Hub only needs to see rover address
  static uint8_t peerAddress[] = {0x3C, 0x8A, 0x1F, 0xA7, 0x1E, 0x28}; //Rover MAC Address
  static ControllerPtr myCurrentController;
#else
  static uint8_t peerAddress[]   = {0x3C, 0x8A, 0x1F, 0xA8, 0x9A, 0x74}; //Hub MAC Address
#endif
static  esp_now_peer_info_t peerInfo;

static data_transmit_t receivedData;
static data_transmit_t dataToSend;

static controller_data_t controllerData;
static ultrasonic_data_t ultrasonicData;

static uint32_t lastSentTime;
static uint32_t retryCount = 0;


void ESP_Now_Transceiver_Init(){
/*
  Rui Santos & Sara Santos - Random Nerd Tutorials
  Complete project details at https://RandomNerdTutorials.com/esp-now-two-way-communication-esp32/
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*/

//Modifications were done on this but it heavily drew inspiration on the initialization fromm the above project

  removeSchedulerEvent(ESP_NOW_INIT_EVENT);
 // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  memcpy(peerInfo.peer_addr, peerAddress, PEER_ADDRESS_SIZE);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));

  addSchedulerEvent(ESP_NOW_WAIT_EVENT);
}

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len){
  /*
  Rui Santos & Sara Santos - Random Nerd Tutorials
  Complete project details at https://RandomNerdTutorials.com/esp-now-two-way-communication-esp32/
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
  */

  char macStr[18];
  Serial.printf("has received data from ");
  snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
  mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  Serial.print(macStr);
  memcpy(&receivedData, incomingData, sizeof(receivedData));

  if(receivedData.dataTransmitType == DATA_TRANSMIT_TYPE_CONTROLLER){
    controllerData = receivedData.controller_data; //Updates the controller data
    ESP_NowPrintControllerData();
  }
}

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status){
  /*
  Rui Santos & Sara Santos - Random Nerd Tutorials
  Complete project details at https://RandomNerdTutorials.com/esp-now-two-way-communication-esp32/
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
  */
  char macStr[18];
  Serial.print("Packet to: ");
  // Copies the sender mac address to a string
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
  Serial.print(" send status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");

  if(status == ESP_NOW_SEND_SUCCESS || retryCount > 5){
    //On Successful send or give up after 5 retries
    clearCount(retryCount);
    removeSchedulerEvent(ESP_RETRY_SEND_EVENT);
    addSchedulerEvent(ESP_WAIT_EVENT);
  }
  else{
    removeSchedulerEvent(ESP_WAIT_EVENT);
    addSchedulerEvent(ESP_RETRY_SEND_EVENT);
    retryCount++;
    //Attempt to retransmit if it failed
  }

}

/*                       HUB CONFIGURATION              */
#if TRANSCEIVER_BUILD == HUB_BUILD
void ESP_NowControllerInit(){
  Serial.printf("Initializing controller:\n");
  removeSchedulerEvent(CONTROLLER_INIT_EVENT);
  controllerInit();
  delay(1000); //make sure the controller actually gets initialized
  ESP_Now_Hub_Pair_Controller();
  addSchedulerEvent(CONTROLLER_CHECK_PAIRING_EVENT);
}

void ESP_Now_Hub_Pair_Controller(){
  if(BP32.update()){//updates Bluepad to check if new connection is available
    removeSchedulerEvent(CONTROLLER_CHECK_PAIRING_EVENT);
    addSchedulerEvent(ESP_NOW_INIT_EVENT);
  } 
}

void ESP_Now_Hub_Check_Controller_Status(){
  if(getControllerStatus()){
    ESP_NowGetController(); //Transmit data on successful controller update
  }
}

void ESP_NowGetController(){
  myCurrentController = getController(); //updates myCurrentController if there is new data
  if(!myCurrentController){return;} //Catches exception of nullptr
  
  static controller_data_t newControllerData;
  
  /* Updates the new controllerDatas */
  newControllerData.dpad = myCurrentController->dpad();
  newControllerData.axisX = myCurrentController->axisX();
  newControllerData.axisY = myCurrentController->axisY();
  newControllerData.axisRX = myCurrentController->axisRX();
  newControllerData.axisRY = myCurrentController->axisRY();
  newControllerData.btnA = myCurrentController->a();
  newControllerData.btnB = myCurrentController->b();
  newControllerData.btnX = myCurrentController->x();
  newControllerData.btnY = myCurrentController->y();
  newControllerData.l1 = myCurrentController->l1();
  newControllerData.l2 = myCurrentController->l2();
  newControllerData.r1 = myCurrentController->r1();
  newControllerData.r2 = myCurrentController->r2();
  newControllerData.thumbL = myCurrentController->thumbL();
  newControllerData.thumbR = myCurrentController->thumbR();

  if(ESP_NowSanitizeController(newControllerData, controllerData)){
    //if the controller data is different from the previous data then we will update the global controllerData
    controllerData = newControllerData; //update controllerData to the new data
    ESP_NowPrintControllerData(); 
    ESP_NowTransmitData(DATA_TRANSMIT_TYPE_CONTROLLER);
  }
}

void ESP_Now_Wait(){
  ESP_Now_Hub_Check_Controller_Status();
  // delay(100);
}


#else /* Rover Transceiver Functions */

void ESP_Now_Wait(){
  // Serial.printf("Waiting for data\n");
  // delay(1000);
}


#endif



void ESP_NowTransmitData(uint32_t type){
  switch (type) {
    case DATA_TRANSMIT_TYPE_CONTROLLER:
      ESP_NowTransmitDataController();
      break;
    default:
      Serial.printf("No valid type was inputted");
      break;
  }
}

void ESP_NowTransmitDataController(){
  dataToSend.dataTransmitType = DATA_TRANSMIT_TYPE_CONTROLLER;
  
  dataToSend.controller_data = controllerData;

  //Note that dataToSend.ultrasonic_data is not used so consider the data to be garbage and do not look there
  esp_now_send(peerAddress, (uint8_t *) &dataToSend, sizeof(data_transmit_t));
  Serial.printf("Attempting to send controller data to address: %02X:%02X:%02X:%02X:%02X:%02X\n",
              peerAddress[0], peerAddress[1], peerAddress[2],
              peerAddress[3], peerAddress[4], peerAddress[5]);  
}

void ESP_NowPrintControllerData(){
  Serial.printf(
    "dpad: 0x%02x, axis L: %4d, %4d, axis R: %4d, %4d, buttons: A %d, B %d, X %d, Y %d, L1 %d, L2 %d, R1 %d, R2 %d, ThumbL %d, ThumbR %d\n",
    controllerData.dpad,           // D-Pad
    controllerData.axisX,          // Left X Axis
    controllerData.axisY,          // Left Y Axis
    controllerData.axisRX,         // Right X Axis
    controllerData.axisRY,         // Right Y Axis
    controllerData.btnA,           // Button A
    controllerData.btnB,           // Button B
    controllerData.btnX,           // Button X
    controllerData.btnY,           // Button Y
    controllerData.l1,             // Button L1
    controllerData.l2,             // Button L2
    controllerData.r1,             // Button R1
    controllerData.r2,             // Button R2
    controllerData.thumbL,         // Left Thumbstick Press
    controllerData.thumbR          // Right Thumbstick Press
  );
}

bool ESP_NowSanitizeController(controller_data_t &newData, const controller_data_t &prevData){
  // if(newData == NULL || prevData == NULL){
  //   return false;
  // }

  if(
  SANTIZATION_CHANGED(newData.axisX, prevData.axisX) ||
  SANTIZATION_CHANGED(newData.axisY, prevData.axisY) ||
  SANTIZATION_CHANGED(newData.axisRX, prevData.axisRX) ||
  SANTIZATION_CHANGED(newData.axisRY, prevData.axisRY) ||
  newData.dpad != prevData.dpad     ||
  newData.btnA != prevData.btnA     ||
  newData.btnB != prevData.btnB     ||
  newData.btnX != prevData.btnX     ||
  newData.btnY != prevData.btnY     ||
  newData.l1   != prevData.l1       ||
  newData.l2   != prevData.l2       ||
  newData.r1   != prevData.r1       ||
  newData.r2   != prevData.r2       ||
  newData.thumbL != prevData.thumbL ||
  newData.thumbR != prevData.thumbR){
    //checks if there is a significant change in the joysticks or if a different button was pressed
    return true;
  }


  return false; //No meaningful change
}
