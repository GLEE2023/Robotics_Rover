#ifndef ESP_NOW_TRANSCEIVER_H
#define ESP_NOW_TRANSCEIVER_H

#include <esp_now.h>
#include <WiFi.h>
#include "Controller.hpp"

#define HUB_BUILD         0
#define ROVER_BUILD       1
#define TRANSCEIVER_BUILD HUB_BUILD
//change the build above to compile for the hub vs the rover

typedef struct controller_data_t{
  uint8_t dpad;
  int axisX;
  int axisY;
  int axisRX;
  int axisRY;
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

/*                     Shared Functions                 */
void ESP_Now_Transceiver_Init(); //Initializes Transceiver

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len);
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);

//Send data commands
void ESP_NowTransmitDataController(); //Transmits the controller data
void ESP_NowTransmitData(uint32_t type);

void ESP_NowPrintControllerData();
bool ESP_NowSanitizeController(controller_data_t &newData, const controller_data_t &prevData); //Checks whether the L and R values were actually changed, as well as removing any controller deadzone

void ESP_Now_Wait();


/*                 HUB FUNCTIONS                */
#if TRANSCEIVER_BUILD == HUB_BUILD
//Controller wrapper functions
void ESP_Now_Hub_Pair_Controller(); //Checks whether a controller is connected
void ESP_Now_Hub_Check_Controller_Status(); //Checks whether a controller has new data 
void ESP_NowControllerInit(); //Initializes the controller pairing
void ESP_NowGetController(); //Updates the global controller if there is an actual change and transmit the data
#endif


#endif
