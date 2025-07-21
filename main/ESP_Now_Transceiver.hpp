#ifndef ESP_NOW_TRANSCEIVER_H
#define ESP_NOW_TRANSCEIVER_H

#include "BuildConfig.hpp"
#include <esp_now.h>
#include <WiFi.h>
#include "Scheduler.hpp"

#if TRANSCEIVER_BUILD == ROVER_BUILD
  #include <esp_wifi.h> 
  /*If its the rover we need to power saving mode but the hub doesn't so we don't need to include this library
  FALSE IF WE WANT TO USE WIFI CHANNEL OTHER THAN 0 THE CODE MUST CHANGE AND WE MUST USE THE FOLLOW WHEN INITIALIZING
  esp_wifi_set_channel(WIFI_CHANNEL, WIFI_SECOND_CHAN_NONE); */
  #include "Motor.hpp"
  #include "HDM.hpp"
  #include "Ultrasonic.hpp"
#else
  #include "Controller.hpp" //Only hub needs to use the controller functions
#endif


#define PEER_ADDRESS_SIZE 6 //numbers of elements which is 6 bytes

//Packet transmission type
#define DATA_TRANSMIT_TYPE_CONTROLLER 0
#define DATA_TRANSMIT_TYPE_ULTRASONIC 1

//Reduce unnecessary sends so santize the controller data
#define SANTIZATION_DEADZONE              50
#define SANTIZATION_CHANGED(new, old)     (abs(abs((new))-abs((old))) > SANTIZATION_DEADZONE) //macro to detect if there is a significant change

#define WIFI_CHANNEL                      0
#define ULTRASONIC_COUNT                  4

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
  int distance[ULTRASONIC_COUNT];
} ultrasonic_data_t; 


typedef struct data_transmit_t{
  uint8_t dataTransmitType;
  controller_data_t controller_data;
  ultrasonic_data_t ultrasonic_data;
} data_transmit_t;

/*                     Shared Functions                 */
void ESP_Now_TransceiverInit(); //Initializes Transceiver

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len);
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);

//Send data commands
void ESP_Now_TransmitData(uint32_t type);

void ESP_Now_PrintUltrasonicData();
void ESP_Now_PrintControllerData();

void ESP_Now_Wait(); //Can be compiled as two different functions

/*                 HUB FUNCTIONS                */
#if TRANSCEIVER_BUILD == HUB_BUILD
//Controller wrapper functions
void ESP_Now_ControllerInit(); //Initializes the controller pairing
void ESP_Now_PairController(); //Checks whether a controller is connected
void ESP_Now_CheckControllerStatus(); //Checks whether a controller has new data 
void ESP_Now_GetController(); //Updates the global controller if there is an actual change and transmit the data
bool ESP_Now_SanitizeController(controller_data_t &newData, const controller_data_t &prevData); //Checks whether the L and R values were actually changed, as well as removing any controller deadzone
void ESP_Now_TransmitDataController(); //Transmits the controller data
#else if TRANSCEIVER_BUILD == ROVER_BUILD
/*             ROVER FUNCTIONS                  */

/* Initializations*/
void ESP_Now_MotorInit();
void ESP_Now_HDMInit();
void ESP_Now_UltrasonicInit();


void ESP_Now_GetUltrasonicData();
void ESP_Now_ParseControllerData();
void ESP_Now_TransmitDataUltrasonic(); //Transmits the ultrasonic data
#endif

#endif
