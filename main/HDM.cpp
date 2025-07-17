#include "HDM.hpp"

void HDMInit(){
  Serial.begin(115200); //Called in main?
  Serial.printf("Initializing UART");//We don't know if Serial.begin(115200) has been declared this is the assumption it has
  Serial1.begin(BAUD_RATE, SERIAL_8N1, RX2, TX2);
}

void HDMSendCommand(String command){
  Serial1.printf("%s", command); //sends the desired command
  delay(100); //wait 0.1 seconds
  HDMStatus(); //Displays to user whether command successfully sent
}

void HDMStatus(){
  if(Serial1.available()){//Checks if there is new data 
    String status = Serial1.readStringUntil('\n'); //Reads data until a new line
    // Serial.println(received); 
    if(status == "Successful"){
      Serial.printf("HDM command delivery: SUCCESS\n");
    }
    else{
      Serial.printf("HDM command sent but error has occured\n");
      //Hopefully never see this message
    }
  }
  else{
    //Data failed to send
    Serial.printf("HDM command delivery: FAILURE\n");
  }
}
