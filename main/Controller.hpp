#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Bluepad32.h"


void onControllerConnected(ControllerPtr newController);
void onControllerDisconnected(ControllerPtr newController);

bool isControllerPaired();  //returns true if controller is paired
bool getControllerStatus(); //returns true if controller has new data

ControllerPtr getController();

void contrllerInit();



#endif 