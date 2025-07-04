#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "BuildConfig.hpp"

/* Only Need to compile this file if HUB_BUILD */
#if TRANSCEIVER_BUILD == HUB_BUILD 
#include "Bluepad32.h"


void onControllerConnect(ControllerPtr newController);
void onControllerDisconnect(ControllerPtr newController);

/* Controller Wrapper Functions for BP32 */
bool updateController(); //calls BP32.update()
bool isControllerPaired();  //returns true if controller is paired
bool hasControllerData(); //returns true if controller has new data
bool getControllerStatus(); //returns true if controller exists, has new data, and is paired

ControllerPtr getController();

void controllerInit();

#endif

#endif 