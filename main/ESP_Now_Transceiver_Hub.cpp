#include "ESP_Now_Transceiver_Hub.hpp"
#include "Scheduler.hpp"

#define PEER_ADDRESS_SIZE 6 //numbers of elements in roverAddress which is 6

#define DATA_TRANSMIT_TYPE_CONTROLLER 0
#define DATA_TRANSMIT_TYPE_ULTRASONIC 1

static uint8_t roverAddress[] = {0x3C, 0x8A, 0x1F, 0xA7, 0x1E, 0x28};
static  esp_now_peer_info_t peerInfo;

static uint8_t receivedData;
static data_transmit_t dataToSend;

static controller_data_t controllerData;

static ControllerPtr myCurrentController;

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
  
  // Register peer
  memcpy(peerInfo.peer_addr, roverAddress, PEER_ADDRESS_SIZE);
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

void ESP_NowControllerInit(){
  Serial.printf("Initializing controller:\n");
  removeSchedulerEvent(CONTROLLER_INIT_EVENT);
  controllerInit();
  delay(1000); //make sure the controller actually gets initialized
  ESP_Now_Hub_Pair_Controller();
  addSchedulerEvent(CONTROLLER_CHECK_PAIRING_EVENT);
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

}

void ESP_Now_Hub_Pair_Controller(){
  if(BP32.update()){//updates Bluepad to check if new connection is available
    removeSchedulerEvent(CONTROLLER_CHECK_PAIRING_EVENT);
    addSchedulerEvent(ESP_NOW_INIT_EVENT);
  } 
  // if(isControllerPaired()){
  //   removeSchedulerEvent(CONTROLLER_CHECK_PAIRING_EVENT);
  //   Serial.printf("Controller has been successfully paired\n");
  //   addSchedulerEvent(ESP_NOW_INIT_EVENT);
  // }
}

void ESP_Now_Hub_Check_Controller_Status(){
  if(getControllerStatus()){
    ESP_NowGetController();
    //Get controller data if new data is available

    //Transmit the data
    ESP_NowTransmitData(DATA_TRANSMIT_TYPE_CONTROLLER);
  }
}

ControllerPtr ESP_NowGetController(){
  myCurrentController = getController(); //updates myCurrentController if there is new data
  if(myCurrentController){
    controllerData.dpad = myCurrentController->dpad();
    controllerData.axisX = myCurrentController->axisX();
    controllerData.axisY = myCurrentController->axisY();
    controllerData.axisRX = myCurrentController->axisRX();
    controllerData.axisRY = myCurrentController->axisRY();
    controllerData.btnA = myCurrentController->a();
    controllerData.btnB = myCurrentController->b();
    controllerData.btnX = myCurrentController->x();
    controllerData.btnY = myCurrentController->y();
    controllerData.l1 = myCurrentController->l1();
    controllerData.l2 = myCurrentController->l2();
    controllerData.r1 = myCurrentController->r1();
    controllerData.r2 = myCurrentController->r2();
    controllerData.thumbL = myCurrentController->thumbL();
    controllerData.thumbR = myCurrentController->thumbR();
  }
}

void ESP_NowTransmitDataController(){
  dataToSend.dataTransmitType = DATA_TRANSMIT_TYPE_CONTROLLER;
  
  dataToSend.controller_data = controllerData;

  //Note that dataToSend.ultrasonic_data is not used so consider the data to be garbage and do not look there
  esp_now_send(roverAddress, (uint8_t *) &dataToSend, sizeof(data_transmit_t));
  Serial.printf("Attempting to send the controller data to device address %d\n", roverAddress);
}

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

void ESP_Now_Hub_Wait(){
  ESP_Now_Hub_Check_Controller_Status();
  delay(100);
}
