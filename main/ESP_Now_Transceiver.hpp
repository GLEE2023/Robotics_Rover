#ifndef ESP_NOW_TRANSCEIVER_HUB_H
#define ESP_NOW_TRANSCEIVER_HUB_H

#include <esp_now.h>
#include <WiFi.h>

#include "Controller.hpp"

typedef struct controller_data_t{
  uint8_t dpad;
  uint32_t axisX;
  uint32_t axisY;
  uint32_t axisRX;
  uint32_t axisRY;
  bool     btnA;
  bool     btnB;
  bool     btnX;
  bool     btnY;
  bool     l1;
  bool     l2;
  bool     r1;
  bool     r2;
  bool     thumbL;
  bool     thumbR;
} controller_data_t;

typedef struct ultrasonic_data_t{
  int data[4];//NUMBEROFSENSORS];
} ultrasonic_data_t;

typedef struct data_transmit_t{
  uint8_t dataTransmitType;
  controller_data_t controller_data;
  ultrasonic_data_t ultrasonic_data;
} data_transmit_t;

void ESP_Now_Transceiver_Init(); //Initializes Transceiver

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len);
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);

//Controller wrapper
void ESP_Now_Hub_Pair_Controller(); //Checks whether a controller is connected
void ESP_Now_Hub_Check_Controller_Status(); //Checks whether a controller has new data 
void ESP_NowControllerInit(); //Initializes the controller pairing

//Send data commands
void ESP_NowTransmitDataController(); //Transmits the controller data
void ESP_NowTransmitData(uint32_t type);

ControllerPtr ESP_NowGetController();

void ESP_Now_Hub_Wait();




#endif