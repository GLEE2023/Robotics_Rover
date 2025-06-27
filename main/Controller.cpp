#include "Controller.hpp"

ControllerPtr myController;

void onControllerConnect(ControllerPtr newController) {
  if(myController == nullptr){
    myController = newController;
    Serial.printf("Connect is successfully paired");
    ControllerProperties properties = newController->getProperties();
    Serial.printf("Controller model: %s, VID=0x%04x, PID=0x%04x\n", newController->getModelName().c_str(), properties.vendor_id, properties.product_id);
  }
  else{
    Serial.printf("Another controller is already paired, unable to pair");
  }
}

void onControllerDisconnect(ControllerPtr newController) {
  if(myController != nullptr){
    Serial.printf("Connect is successfully disconnected");
    ControllerProperties properties = newController->getProperties();
    Serial.printf("Controller model: %s, VID=0x%04x, PID=0x%04x\n", newController->getModelName().c_str(), properties.vendor_id, properties.product_id);
    myController = nullptr;
  }
  else{
    Serial.printf("Controller was not connected");
  }
}

void controllerInit() {
  Serial.printf("Firmware: %s\n", BP32.firmwareVersion());
  const uint8_t* addr = BP32.localBdAddress();
  Serial.printf("BD Addr: %2X:%2X:%2X:%2X:%2X:%2X\n", addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);

  // Setup the Bluepad32 callbacks
  BP32.setup(&onControllerConnect, &onControllerDisconnect);

  // "forgetBluetoothKeys()" should be called when the user performs
  // a "device factory reset", or similar.
  // Calling "forgetBluetoothKeys" in setup() just as an example.
  // Forgetting Bluetooth keys prevents "paired" gamepads to reconnect.
  // But it might also fix some connection / re-connection issues.
  BP32.forgetBluetoothKeys();

  // Enables mouse / touchpad support for gamepads that support them.
  // When enabled, controllers like DualSense and DualShock4 generate two connected devices:
  // - First one: the gamepad
  // - Second one, which is a "virtual device", is a mouse.
  // By default, it is disabled.
  BP32.enableVirtualDevice(false);
}

bool isControllerPaired(){
  if(!myController){
    Serial.printf("Waiting for controller to pair");
    return false;
  }
  return true;
}

bool getControllerStatus(){
  return BP32.update(); //returns whether controller has new data
}

ControllerPtr getController(){
  return myController;
}
