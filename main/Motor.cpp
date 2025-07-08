#include "Arduino.h"
void motorInit(){

  /* PWM Pins Init */
  pinMode(MOTOR_ONE_PWM_PIN, OUTPUT);
  pinMode(MOTOR_TWO_PWM_PIN, OUTPUT);
  pinMode(MOTOR_THREE_PWM_PIN, OUTPUT);
  pinMode(MOTOR_FOUR_PWM_PIN, OUTPUT);

  /* ENC PINS INIT */
  pinMode(MOTOR_ONE_ENC_PIN, INPUT_PULLUP); /*NE12 datasheet says it is open-collect ref: https://www.servocity.com/content/downloads/ne12_-_use_parameter.pdf */
  pinMode(MOTOR_TWO_ENC_PIN, INPUT_PULLUP);
  pinMode(MOTOR_THREE_ENC_PIN, INPUT_PULLUP);
  pinMode(MOTOR_FOUR_ENC_PIN, INPUT_PULLUP);

  /* Direction Controls Init*/
  pinMode(MOTOR_LEFT_DIR_PIN, OUTPUT);
  pinMode(MOTOR_RIGHT_DIR_PIN, OUTPUT);

}


uint32_t getPulseInterval(uint32_t motorNUM){
  noInterrupts(); //Disable interrupts temporarily
  uint32_t temp = pulseInterval[motorNUM]; //Copies the data 
  interrupts();
  return temp;
}

