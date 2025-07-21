#include "esp32-hal.h"
#include "Ultrasonic.hpp"
#include <Arduino.h>

const int echoPin[ULTRASONIC_COUNT] = {ECHO_PIN_ZERO, ECHO_PIN_ONE, ECHO_PIN_TWO, ECHO_PIN_THREE};


unsigned long   duration = 0;
float           distanceInch[ULTRASONIC_COUNT] = {0}; 

void ultrasonicInit(){
  pinMode(TRIG_PIN, OUTPUT); 

  /* Initializes ECHO pins */
  pinMode(ECHO_PIN_ZERO, INPUT);
  pinMode(ECHO_PIN_ONE, INPUT); 
  pinMode(ECHO_PIN_TWO, INPUT); 
  pinMode(ECHO_PIN_THREE, INPUT); 

  digitalWrite(TRIG_PIN, LOW);
}

void updateUltrasonicSensors(){
  for(int index = 0; index < ULTRASONIC_COUNT; index++){

    //Enable Trig pin for 10 microseconds
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    duration = pulseIn(echoPin[index], HIGH, 50000); //timeout after 50 ms
   
    if(duration == 0){
      distanceInch[index] = -1; //Timeout occured
    }
    else{
      distanceInch[index] = duration * SOUND_SPEED;
    }


    // DEBUG
    // Serial.printf("Sensor number %d has a distance of %d (inch)", index, distanceInch[index]);

    delay(100); //wait 100ms before getting the next sensor distance
  }
}

void getUltrasonic(float distances[]){
  for (int i = 0; i < ULTRASONIC_COUNT; i++) {
        distances[i] = distanceInch[i];
  }
}