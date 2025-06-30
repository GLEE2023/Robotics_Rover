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

//Send data commands
void ESP_NowTransmitData(uint32_t type);

void ESP_Now_Hub_Wait();


#endif