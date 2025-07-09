#include "Motor.hpp"

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


  /* Creates ISR for encoders */
  attachInterrupt(digitalPinToInterrupt(MOTOR_ONE_ENC_PIN), motorOneEncoderISR, RISING);
  attachInterrupt(digitalPinToInterrupt(MOTOR_TWO_ENC_PIN), motorTwoEncoderISR, RISING);
  attachInterrupt(digitalPinToInterrupt(MOTOR_THREE_ENC_PIN), motorThreeEncoderISR, RISING);
  attachInterrupt(digitalPinToInterrupt(MOTOR_FOUR_ENC_PIN), motorFourEncoderISR, RISING);
}

void updateDesiredRPM(float newSpeed){
  desiredRPM = newSpeed;
}

void matchDesiredRPM(){
  for(int i = 0; i<MOTOR_COUNT; i++){
    motorActualRPM[i] = calculateRPM(i);
    motorOutputVoltage[i] = motorOutputVoltage[i] + ((desiredRPM - motorActualRPM[i]) / desiredRPM);
    /* this logic was given, it can be expanded upon in the future and should be a PID controller 
    It normalizes the error (the stuff on the right) and adds it to the actual (the error could be a positive or negative value)*/
    motorOutputVoltage[i] = constrain(motorOutputVoltage[i], 0, MAX_VOLTAGE); //Sets the output voltage to a minimum of 0 or a max of 3.3
    analogWrite(motorPWMPIN[i], motorOutputVoltage[i] * 51);
  }
}

uint32_t getPulsePeriod(uint32_t motorNum){
  noInterrupts(); //Disable interrupts temporarily
  uint32_t tempPeriod = motorPulsePeriod[motorNum]; //Copies the data 
  interrupts();
  return tempPeriod;
}

uint32_t calculateRPM(uint32_t motorNum){ //take in a given motor and returns its rpm
  uint32_t period = getPulsePeriod(motorNum); //gets the period
  if(period == 0){
    return 0; //motor startup so ignore this otherwise we have divide by 0
  }
  float freq = MICROS_PER_SECOND / period; //gets the frequency in Hz
  float rpm  = RPM_FROM_FREQ(freq); //calculates rpm
  return rpm;
}

void IRAM_ATTR motorOneEncoderISR(){
  uint32_t currentTime = micros();
  motorPulsePeriod[MOTOR_ONE] = currentTime - motorLastPulseTime[MOTOR_ONE];
  motorLastPulseTime[MOTOR_ONE] = currentTime;
}
void IRAM_ATTR motorTwoEncoderISR(){
  uint32_t currentTime = micros();
  motorPulsePeriod[MOTOR_TWO] = currentTime - motorLastPulseTime[MOTOR_TWO];
  motorLastPulseTime[MOTOR_TWO] = currentTime;
}
void IRAM_ATTR motorThreeEncoderISR(){
  uint32_t currentTime = micros();
  motorPulsePeriod[MOTOR_THREE] = currentTime - motorLastPulseTime[MOTOR_THREE];
  motorLastPulseTime[MOTOR_THREE] = currentTime;
}

void IRAM_ATTR motorFourEncoderISR(){
  uint32_t currentTime = micros();
  motorPulsePeriod[MOTOR_FOUR] = currentTime - motorLastPulseTime[MOTOR_FOUR];
  motorLastPulseTime[MOTOR_FOUR] = currentTime;
}