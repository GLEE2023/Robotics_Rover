#line 1 "C:\\Users\\Ethan Levin\\Documents\\Arduino\\Controller\\ESP_Now_Transceive_Hub.hpp"
#ifndef ESP_NOW_TRANSCEIVE_HUB_H
#define ESP_NOW_TRANSCEIVE_HUB_H

#include <esp_now.h>
#include <WiFi.h>

#include "Controller.hpp"

// typedef struct ultrasonic_message_t{
//   float distance;
//   int number; //which ultrasonic sensor this data is coming from
// } ultrasonic_message_t;

// typedef struct controller_message{
//   ControllerPtr info;
// } controller_message_t;

void ESP_Now_Transceive_Init(); //Initializes Transceiver

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len);
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);

//Controller wrapper
void ESP_Now_Hub_Pair_Controller(); //Checks whether a controller is connected
void ESP_Now_Hub_Check_Controller_Status(); //Checks whether a controller has new data 
void ESP_NowControllerInit(); //Initializes the controller pairing

//Send data commands
void ESP_NowTransmitControllerData(ControllerPtr myCtrl); //Transmits the controller data

ControllerPtr ESP_NowGetController();

void ESP_Now_Hub_Wait();




#endif