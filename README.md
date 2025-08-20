Libraries required:

Bluepad32 by Ricardo Quesada: 
  To add these libraries go to file->preferences and paste the two links below in Additional boards manager URLs.
    https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
    https://raw.githubusercontent.com/ricardoquesada/esp32-arduino-lib-builder/master/bluepad32_files/package_esp32_bluepad32_index.json

esp32 by Espressif Systems

The code can be compiled for either the Hub microcontroller or Rover microcontroller. Before downloading the code go to BuildConfig.hpp and change TRANSCEIVER_BUILD to either HUB_BUILD or ROVER_BUILD.

The Hub microcontroller compilation uses Bluepad32 (a Bluetooth protocol) to read in data from a controller. It then uses ESP-NOW to send it to the Rover microcontroller. The Hub configuration should use the esp32 dev module by esp32_bluepad32 board when downloading the code. This is seen in the image below.

<img width="680" height="481" alt="image" src="https://github.com/user-attachments/assets/e7f2c710-330a-4fed-a900-ffb8cef27f17" />

The Rover microcontroller configuration solely uses the ESP-NOW protocol. Additionally, to minimize the packet loss that occurs from running both Bluetooth and ESP-NOW at the same time, the rover microcontroller is set to disable power saving mode. This option cannot be disabled if both Bluetooth and ESP-NOW are running and since bluepad32 automatically enables Bluetooth, we cannot use that board configuration. The correct board that should be selected is  esp32 dev module by esp32. This can be seen in the image below.

<img width="686" height="494" alt="image" src="https://github.com/user-attachments/assets/4a4868bc-4a57-480b-ad3e-df521ae77450" />
