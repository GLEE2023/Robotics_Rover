#include "ESP_Now_Transceiver_Rover.hpp"


#define PEER_ADDRESS_SIZE 6 //sizeof(receiverAddress) which is 6

static uint8_t hubAddress[] = {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA};

// received_command;

// void ESP_Now_Transceive_Init(){
// /*
//   Rui Santos & Sara Santos - Random Nerd Tutorials
//   Complete project details at https://RandomNerdTutorials.com/esp-now-two-way-communication-esp32/
//   Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
//   The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
// */

// //Modifications were done on this but it heavily drew inspiration on the initialization fromm the above project


//  // Set device as a Wi-Fi Station
//   WiFi.mode(WIFI_STA);

//   // Init ESP-NOW
//   if (esp_now_init() != ESP_OK) {
//     Serial.println("Error initializing ESP-NOW");
//     return;
//   }

//   // Once ESPNow is successfully Init, we will register for Send CB to
//   // get the status of Trasnmitted packet
//   esp_now_register_send_cb(OnDataSent);
  
//   // Register peer
//   memcpy(peerInfo.peer_addr, receiverAddress, PEER_ADDRESS_SIZE);
//   peerInfo.channel = 0;  
//   peerInfo.encrypt = false;
  
//   // Add peer        
//   if (esp_now_add_peer(&peerInfo) != ESP_OK){
//     Serial.println("Failed to add peer");
//     return;
//   }
//   // Register for a callback function that will be called when data is received
//   esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
// }

// void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len){
//   /*
//   Rui Santos & Sara Santos - Random Nerd Tutorials
//   Complete project details at https://RandomNerdTutorials.com/esp-now-two-way-communication-esp32/
//   Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
//   The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//   */

//   //The function definition is the same but the actual function is different
  
//   memcpy(&received_command, incomingData, sizeof(received_command));

// }

// void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status){
//   /*
//   Rui Santos & Sara Santos - Random Nerd Tutorials
//   Complete project details at https://RandomNerdTutorials.com/esp-now-two-way-communication-esp32/
//   Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
//   The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//   */
//   char macStr[18];
//   Serial.print("Packet to: ");
//   // Copies the sender mac address to a string
//   snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
//            mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
//   Serial.print(macStr);
//   Serial.print(" send status:\t");
//   Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
// }


// void ESP_Now_Rover_Wait(){
  
// }
