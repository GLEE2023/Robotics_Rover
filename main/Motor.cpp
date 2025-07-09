#include "Motor.hpp"

const uint8_t motorPWMPin[MOTOR_COUNT] = {MOTOR_ONE_PWM_PIN, MOTOR_TWO_PWM_PIN, MOTOR_THREE_PWM_PIN, MOTOR_FOUR_PWM_PIN};

volatile uint32_t motorPulsePeriod[MOTOR_COUNT] = {0}; //Time between last encoder pulse and current encoder plse
volatile uint32_t motorLastPulseTime[MOTOR_COUNT] = {0}; 
volatile int desiredRPM = 0;

float motorActualRPM[MOTOR_COUNT] = {0}; //Actual RPM calculated from the encoder pulses
float motorOutputVoltage[MOTOR_COUNT] = {0}; //Outputted PWM to get the desired RPM

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

void motorDriveLeft(uint8_t dir){
  motorDrive(LEFT_SIDE, dir);
}

void motorDriveRight(uint8_t dir){
  motorDrive(RIGHT_SIDE, dir);
}

void motorDrive(uint8_t side, uint8_t dir){
  if(side == LEFT_SIDE){
    Serial.printf("Left side is attempting to %s\n", dir == FORWARDS ? "forwards" : "backwards");
    digitalWrite(motorPWMPin[MOTOR_ONE], dir);
    digitalWrite(motorPWMPin[MOTOR_TWO], dir);
  }
  else{ /* side = RIGHT_SIDE*/
    Serial.printf("Right side is attempting to %s\n", dir == FORWARDS ? "forwards" : "backwards");
    digitalWrite(motorPWMPin[MOTOR_THREE], dir);
    digitalWrite(motorPWMPin[MOTOR_FOUR], dir);
  }
}

void updateDesiredRPM(int change){
  desiredRPM += 5*change;
  desiredRPM = constrain(desiredRPM, 0, 30);
  Serial.printf("Changing speed by %d\n New speed is %d\n", 5*change, desiredRPM);
}

void matchDesiredRPM(){
  for(int i = 0; i<MOTOR_COUNT; i++){
    motorActualRPM[i] = calculateRPM(i);
    motorOutputVoltage[i] = motorOutputVoltage[i] + ((desiredRPM - motorActualRPM[i]) / desiredRPM);
    /* this logic was given by Frankie Sharman, it can be expanded upon in the future and should be a PID controller 
    It normalizes the error (the stuff on the right) and adds it to the actual (the error could be a positive or negative value)*/
    motorOutputVoltage[i] = constrain(motorOutputVoltage[i], 0, MAX_VOLTAGE); //Sets the output voltage to a minimum of 0 or a max of 3.3
    analogWrite(motorPWMPin[i], motorOutputVoltage[i] * 51);
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