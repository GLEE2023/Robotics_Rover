#include "HDM.hpp"

void HDMInit(){
  Serial.begin(115200); //Called in main?
  Serial.printf("Initializing UART\n");//We don't know if Serial.begin(115200) has been declared this is the assumption it has
  Serial1.begin(BAUD_RATE, SERIAL_8N1, RX2, TX2);
}

void HDMSendCommand(HDMCommand command){
  Serial1.write((uint8_t)command); //sends the desired command
  delay(100); //wait 0.1 seconds
  // HDMStatus(); //Displays to user whether command successfully sent 
  //Dont want status right now
}

void HDMStatus(){
  if(Serial1.available()){//Checks if there is new data 
    String status = Serial1.readString(); //Reads data until a new line
    status.trim();
    Serial.println(status); 
    if(status == "S"){
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
