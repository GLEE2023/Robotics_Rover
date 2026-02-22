# **Libraries required:**

Bluepad32 by Ricardo Quesada: 
  To add these libraries go to file->preferences and paste the two links below in Additional boards manager URLs.

  esp32 by Espressif Systems and Bluepad32
    
    https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
    
    https://raw.githubusercontent.com/ricardoquesada/esp32-arduino-lib-builder/master/bluepad32_files/package_esp32_bluepad32_index.json

Click the box shown in the image below

<img width="962" height="617" alt="image" src="https://github.com/user-attachments/assets/b7edbc3a-0c60-41c4-b282-2e636d141214" />


# **Compile Erorr**

If you see the following error you must resolve it in the steps below.
  
    esp32:esp-x32@2511
    Failed to install platform: 'esp32:esp32:3.3.7'.
    Error: 4 DEADLINE_EXCEEDED: net/http: request canceled
    (Client.Timeout or context cancellation while reading body)

Open arduino-cli.yaml in a text editor WITH the Arduino IDE already open. The filepath is as follows: 

Linux/Mac: ~/.arduinoIDE/arduino-cli.yaml 

Windows: C:\Users\YourUsername\.arduinoIDE\arduino-cli.yaml

<img width="802" height="491" alt="image" src="https://github.com/user-attachments/assets/3c0a87d1-f403-4292-bd56-ccc6009727e9" />

After you open this document you will need to add the following lines of code

    network:
    connection_timeout: 600s

**Make sure to indent connection_timeout: 600s**

The image below shows what the corrected file should look like

<img width="1372" height="320" alt="image" src="https://github.com/user-attachments/assets/ae099291-9980-4361-89ee-119107875126" />

### **AFTER YOU INSTALL THE LIBRARIES GO BACK INTO THE arduino-cli.yamkl FILE AND REMOVE THE CODE WE ADDED OR ELSE THE IDE MAY NOT BOOT UP IN THE FUTURE**


# **COMPILING THE CODE**

The code can be compiled for either the Hub microcontroller or Rover microcontroller. Before downloading the code go to BuildConfig.hpp and change TRANSCEIVER_BUILD to either HUB_BUILD or ROVER_BUILD.

The Hub microcontroller compilation uses Bluepad32 (a Bluetooth protocol) to read in data from a controller. It then uses ESP-NOW to send it to the Rover microcontroller. The Hub configuration should use the esp32 dev module by esp32_bluepad32 board when downloading the code. This is seen in the image below.

<img width="680" height="481" alt="image" src="https://github.com/user-attachments/assets/e7f2c710-330a-4fed-a900-ffb8cef27f17" />

The Rover microcontroller configuration solely uses the ESP-NOW protocol. Additionally, to minimize the packet loss that occurs from running both Bluetooth and ESP-NOW at the same time, the rover microcontroller is set to disable power saving mode. This option cannot be disabled if both Bluetooth and ESP-NOW are running and since bluepad32 automatically enables Bluetooth, we cannot use that board configuration. The correct board that should be selected is  esp32 dev module by esp32. This can be seen in the image below.

<img width="686" height="494" alt="image" src="https://github.com/user-attachments/assets/4a4868bc-4a57-480b-ad3e-df521ae77450" />
