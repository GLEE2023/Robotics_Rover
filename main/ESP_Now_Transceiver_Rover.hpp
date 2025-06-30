#ifndef ESP_NOW_TRANSCEIVE_ROVER_H
#define ESP_NOW_TRANSCEIVE_ROVER_H

#include <esp_now.h>
#include <WiFi.h>


// typedef struct ultrasonic_message_t{
//   float distance
// } ultrasonic_message_t;

// typedef struct controller_message{
//   ControllerPtr info;
// } controller_message_t;



void ESP_Now_Transceiver_Init(); //Initializes Transceiver

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len);
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);

void ESP_Now_Rover_Wait();


#endif