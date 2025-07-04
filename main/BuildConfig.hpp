#ifndef BUILDCONFIG_H
#define BUILDCONFIG_H

#define HUB_BUILD         0  /*!!IMPORTANT!! Compile ESP32 Dev Module as esp32_bluepad32 */
#define ROVER_BUILD       1  /*!!IMPORTANT!! Compile ESP32 Dev Module as just esp32, this is for power saving mode */
// #define TRANSCEIVER_BUILD HUB_BUILD
#define TRANSCEIVER_BUILD HUB_BUILD
//change the build above to compile for the hub vs the rover

#endif