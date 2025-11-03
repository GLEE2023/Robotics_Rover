#include "ESP_Now_Transceiver.hpp"


#if TRANSCEIVER_BUILD == HUB_BUILD //Hub only needs to see rover address
  // static uint8_t peerAddress[] = {0x3C, 0x8A, 0x1F, 0xA7, 0x1E, 0x28}; //Rover MAC Address dead
  // static uint8_t peerAddress[] = {0x6C, 0xC8, 0x40, 0x4f, 0xD9, 0x10}; //Rover MAC Address dead
  // static uint8_t peerAddress[] = {0x6C, 0xC8, 0x40, 0x86, 0x42, 0x24}; //Rover MAC Address dead
    // static uint8_t peerAddress[]   = {0x3C, 0x8A, 0x1F, 0xA8, 0x9A, 0x74}; //Rover MAC Address Retired
    static uint8_t peerAddress[]   = {0x38, 0x18, 0x2B, 0xB2, 0x9D, 0xA8}; //Rover MAC Address Active38:18:2b:b2:9d:a8
    static ControllerPtr myCurrentController;
    static int hubHDMPowerLevel = 0;
    static int hubHDMDiskAmount = 0;
    static controller_data_t currentSentControllerData = {};
    static controller_data_t prevSentControllerData = {}; //0 initialized as static
#else
  // static uint8_t peerAddress[]   = {0x3C, 0x8A, 0x1F, 0xA8, 0x9A, 0x74}; //Hub MAC Address
  static uint8_t peerAddress[]   = {0x6C, 0xC8, 0x40, 0x87, 0x63, 0x34}; //Hub MAC Address

#endif
static  esp_now_peer_info_t peerInfo;

static data_transmit_t receivedData;
static data_transmit_t dataToSend;

static controller_data_t controllerData;
static ultrasonic_data_t ultrasonicData;

static bool newControllerData = false;

void ESP_Now_TransceiverInit(){
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

  #if TRANSCEIVER_BUILD == ROVER_BUILD
  //Disable power saving mode for the rover, unfortunately this cant be done on the hub since we require this mode if we are using both WiFi and Bluetooth
    esp_wifi_set_ps(WIFI_PS_NONE);
  #endif

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  memcpy(peerInfo.peer_addr, peerAddress, PEER_ADDRESS_SIZE);
  peerInfo.channel = WIFI_CHANNEL;  
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
  Serial.printf("\n");
  memcpy(&receivedData, incomingData, sizeof(receivedData));

#if TRANSCEIVER_BUILD == HUB_BUILD
  if(receivedData.dataTransmitType == DATA_TRANSMIT_TYPE_ULTRASONIC){
    ultrasonicData = receivedData.ultrasonic_data;
    ESP_Now_PrintUltrasonicData();
  }
#elif TRANSCEIVER_BUILD == ROVER_BUILD
  if(receivedData.dataTransmitType == DATA_TRANSMIT_TYPE_CONTROLLER){
    controllerData = receivedData.controller_data; //Updates the controller data
    newControllerData = true;
    ESP_Now_PrintControllerData();
  }
#endif
  else{
    Serial.printf("Unknown data transmission type\n");
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
}

void ESP_Now_TransmitData(uint32_t type){
  switch (type) {
#if TRANSCEIVER_BUILD == HUB_BUILD
    case DATA_TRANSMIT_TYPE_CONTROLLER:
      ESP_Now_TransmitDataController();
      break;
#elif TRANSCEIVER_BUILD == ROVER_BUILD
    case DATA_TRANSMIT_TYPE_ULTRASONIC:
      ESP_Now_TransmitDataUltrasonic();
      break;
#endif
    default:
      Serial.printf("No valid type was inputted\n");
      break;
  }
}

void ESP_Now_PrintUltrasonicData(){
  for(int i = 0; i < ULTRASONIC_COUNT; i++){
    Serial.printf("Ultrasonic sensor #%d, distance: %f\n", i, ultrasonicData.distance[i]);
  }
}

void ESP_Now_PrintControllerData(){
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

/*                       HUB CONFIGURATION              */
#if TRANSCEIVER_BUILD == HUB_BUILD
void ESP_Now_ControllerInit(){
  Serial.printf("Initializing controller:\n");
  removeSchedulerEvent(CONTROLLER_INIT_EVENT);
  controllerInit();
  Serial.printf("Finished Initializing controller\n");
  Serial.printf("Attempting to find controller\n");
  addSchedulerEvent(CONTROLLER_CHECK_PAIRING_EVENT);
}

void  ESP_Now_TransmitDataController(){
  dataToSend.dataTransmitType = DATA_TRANSMIT_TYPE_CONTROLLER;
  dataToSend.controller_data = controllerData;
  //Note that dataToSend.ultrasonic_data is not used so consider the data to be garbage and do not use it
  
  
  ESP_Now_PrintControllerData(); //Data that is going to be sent out

  Serial.printf("Attempting to send controller data to address: %02X:%02X:%02X:%02X:%02X:%02X\n",
              peerAddress[0], peerAddress[1], peerAddress[2],
              peerAddress[3], peerAddress[4], peerAddress[5]);  

  /* DEBUG FOR THE STATUS OF THE SEND */
  esp_err_t result = esp_now_send(peerAddress, (uint8_t *) &dataToSend, sizeof(data_transmit_t));
  // Serial.printf("[DEBUG]: esp_now_send result: %d\n", result);//print if an error occured and of what type

   #if TRANSCEIVER_BUILD == HUB_BUILD
  //   //Only print to terminal for the hub
  //   if(result == ESP_OK){
  //     currentSentControllerData = controllerData; //stores controllerData before we attempt to send
  //     if(prevSentControllerData.l1 != currentSentControllerData.l1){
  //       if(prevSentControllerData.l1 == 1){
            
  //       }
  //     }

  //     prevSentControllerData = currentSentControllerData;
  //   }
    #endif
}

void ESP_Now_PairController(){
  if(updateController()){ //Checks if controller is paired and if it is then change events
    removeSchedulerEvent(CONTROLLER_CHECK_PAIRING_EVENT);
    Serial.println("Successfully paired controller, initializing ESP NOW");
    addSchedulerEvent(ESP_NOW_INIT_EVENT);
  } 
}

void ESP_Now_CheckControllerStatus(){
  if(getControllerStatus()){
    ESP_Now_GetController(); //Transmit data on successful controller update
  }
}

void ESP_Now_GetController(){
  myCurrentController = getController(); //updates myCurrentController if there is new data
  if(!myCurrentController){return;} //Catches exception of nullptr
  
  static controller_data_t newControllerData;
  /* Updates the newcontrollerData */
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

  if(ESP_Now_SanitizeController(newControllerData, controllerData)){
    //if the controller data is different enough from the previous data then we will update the global controllerData
    controllerData = newControllerData; //update controllerData to the new data
    ESP_Now_TransmitData(DATA_TRANSMIT_TYPE_CONTROLLER);
    delay(200);
  }
}

bool ESP_Now_SanitizeController(controller_data_t &newData, const controller_data_t &prevData){
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

void ESP_Now_Wait(){
  ESP_Now_CheckControllerStatus();
}

#else /* Rover Transceiver Functions */

void ESP_Now_MotorInit(){
  removeSchedulerEvent(MOTOR_INIT_EVENT);
  motorInit();
  Serial.println("Finished initializing motors");
  addSchedulerEvent(HDM_INIT_EVENT);//change to ultrasonic init event and remember HDM
}

void ESP_Now_HDMInit(){
  removeSchedulerEvent(HDM_INIT_EVENT);
  HDMInit(); 
  Serial.println("Finished initializing HDM");
  addSchedulerEvent(ULTRASONIC_INIT_EVENT);//change to ultrasonic init event
}

void ESP_Now_UltrasonicInit(){
  removeSchedulerEvent(ULTRASONIC_INIT_EVENT);
  // ultrasonicInit();
  // Serial.println("Finished initializing ultrasonic");
  // Serial.println("Initializing ultrasonic timer");
  // timerUltrasonicInit();
  // Serial.println("Finished initializing ultrasonic timer");
  addSchedulerEvent(ESP_NOW_INIT_EVENT);
}

void ESP_Now_TransmitDataUltrasonic(){
  dataToSend.dataTransmitType = DATA_TRANSMIT_TYPE_ULTRASONIC;
  /*Note that dataToSend.controller_data is not used so consider the data to be garbage and do not use it
  In the future the received data could be transmitted back for error detection*/
  dataToSend.ultrasonic_data = ultrasonicData;

  /* DEBUG FOR THE STATUS OF THE SEND */
  esp_err_t result = esp_now_send(peerAddress, (uint8_t *) &dataToSend, sizeof(data_transmit_t));
  Serial.printf("[DEBUG]: esp_now_send result: %d\n", result);//print if an error occured and of what type
  
  ESP_Now_PrintUltrasonicData(); //Data that is going to be sent out
  
  Serial.printf("Attempting to send controller data to address: %02X:%02X:%02X:%02X:%02X:%02X\n",
              peerAddress[0], peerAddress[1], peerAddress[2],
              peerAddress[3], peerAddress[4], peerAddress[5]);
}

void ESP_Now_Wait(){
  if(newControllerData == true){
    newControllerData = false;
    ESP_Now_ParseControllerData();
  }
  matchDesiredRPM();
}

void ESP_Now_GetUltrasonicData(){
  removeSchedulerEvent(ULTRASONIC_SEND_EVENT);
  getUltrasonic(ultrasonicData.distance);
  ESP_Now_TransmitData(DATA_TRANSMIT_TYPE_ULTRASONIC);
}

void ESP_Now_ParseControllerData(){
  static controller_data_t prevControllerData = {}; //previous controller data from last transmission, note 0 initialized because its statis
  controller_data_t recvControllerData = controllerData; /*temp variable in the event that controllerData changes on from ESP-NOW*/
  static int rpm = 0;
  if(recvControllerData.axisY > 50){
    if(recvControllerData.axisY > 256){
      setDesiredRPM(LEFT_SIDE, 30);
    }
    else{
      setDesiredRPM(LEFT_SIDE, 10);
    }
    //  rpm = map(rpm, 50, 512, 0, 45);
      // setDesiredRPM(LEFT_SIDE, rpm);
          // Serial.printf("Setting left RPM to %d", rpm);

    // if(prevControllerData.axisY < -50){//if it was going forwards then we need to slow down before we change directions
    //   rampDown(LEFT_SIDE);
    //   delay(100);
    // } 
    motorDriveLeft(BACKWARDS);
  }
  else if(recvControllerData.axisY < -50){ 
    if(recvControllerData.axisY < -256){
      setDesiredRPM(LEFT_SIDE, 30);
    }
    else{
      setDesiredRPM(LEFT_SIDE, 10);
    }
    //  rpm = map(rpm, -512, -50, 0, 45);
    // setDesiredRPM(LEFT_SIDE, rpm);
    // Serial.printf("Setting left RPM to %d", rpm);

    // if(prevControllerData.axisY > -50){
    //   rampDown(LEFT_SIDE);
    //   delay(100);
    // } 
    motorDriveLeft(FORWARDS);
  }
  else{
    //set rpm to 0 if joystick in neutral position
    setDesiredRPM(LEFT_SIDE, 0);
  }


  if(recvControllerData.axisRY > 50){
    if(recvControllerData.axisRY > 256){
      setDesiredRPM(RIGHT_SIDE, 30);
    }
    else{
      setDesiredRPM(RIGHT_SIDE, 10);
    }
    //  rpm = map(rpm, 50, 512, 0, 45);
    // setDesiredRPM(RIGHT_SIDE, rpm);
    // Serial.printf("Setting right RPM to %d", rpm);
    //  if(prevControllerData.axisRY < -50){
    //   rampDown(RIGHT_SIDE);
    //   delay(100);
    // } 
    motorDriveRight(BACKWARDS);
  }
  else if (recvControllerData.axisRY < -50){
    if(recvControllerData.axisRY < -256){
      setDesiredRPM(RIGHT_SIDE, 30);
    }
    else{
      setDesiredRPM(RIGHT_SIDE, 10);
    }
    //  rpm = map(rpm, -512, -50, 0, 45);
    // setDesiredRPM(RIGHT_SIDE, rpm);
    // Serial.printf("Setting right RPM to %d", rpm);
    // if(prevControllerData.axisRY > 50){
    //   rampDown(RIGHT_SIDE);
    //   delay(100);
    // } 
    motorDriveRight(FORWARDS);
  }
  else{
    //set RPM to 0 if joystick in neutral position
     setDesiredRPM(RIGHT_SIDE, 0);
  }

  // if(prevControllerData.l1 != recvControllerData.l1){//Prevents triggering twice (on trigger and release)
  //   if(recvControllerData.l1 == 1){
  //     updateDesiredRPMLeft(INCREASE_SPEED);
  //   }
  // }
  // if(prevControllerData.l2 != recvControllerData.l2){
  //   if(recvControllerData.l2 == 1){
  //     updateDesiredRPMLeft(DECREASE_SPEED);
  //   }
  // }
  // if(prevControllerData.r1 != recvControllerData.r1){
  //   if(recvControllerData.r1 == 1){
  //     updateDesiredRPMRight(INCREASE_SPEED);
  //   }
  // }
  // if(prevControllerData.r2 != recvControllerData.r2){
  //   if(recvControllerData.r2 == 1){
  //     updateDesiredRPMRight(DECREASE_SPEED);
  //   }
  // }

  /* HDM Operations */
  if(prevControllerData.l1 != recvControllerData.l1){
    if(recvControllerData.l1 == 1){
      HDMSendCommand(HDM_COMMAND_POWERUP);
    }
  }

  if(prevControllerData.l2 != recvControllerData.l2){
    if(recvControllerData.l2 == 1){
      HDMSendCommand(HDM_COMMAND_POWERDOWN);
    }
  }

  if(prevControllerData.r1 != recvControllerData.r1){
    if(recvControllerData.r1 == 1){
      HDMSendCommand(HDM_COMMAND_INCREMENT_DISKS);
    }
  }

  if(prevControllerData.r2 != recvControllerData.r2){
    if(recvControllerData.r2 == 1){
      HDMSendCommand(HDM_COMMAND_DECREMENT_DISKS);
    }
  }

  if(prevControllerData.btnA != recvControllerData.btnA){ 
    if(recvControllerData.btnA == 1){
      HDMSendCommand(HDM_COMMAND_LAUNCH_DISKS);
    }
  }

  if(prevControllerData.btnB != recvControllerData.btnB){ 
    if(recvControllerData.btnB == 1){
      HDMSendCommand(HDM_COMMAND_ROTATE_BARREL);
    }
  }

  // Serial.println("Parsed Controller data");
  //Update prev controller
  prevControllerData = recvControllerData;
}

#endif