#include "Motor.hpp"

const uint8_t motorPWMPin[MOTOR_COUNT] = {MOTOR_ONE_PWM_PIN, MOTOR_TWO_PWM_PIN, MOTOR_THREE_PWM_PIN, MOTOR_FOUR_PWM_PIN};

volatile uint32_t motorPulsePeriod[MOTOR_COUNT] = {0}; //Time between last encoder pulse and current encoder plse
volatile uint32_t motorLastPulseTime[MOTOR_COUNT] = {0}; 
volatile int desiredRPM = 0;
static int wheelRPM     = 0;

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
    digitalWrite(MOTOR_LEFT_DIR_PIN, dir);
  }
  else{ /* side = RIGHT_SIDE*/
    Serial.printf("Right side is attempting to %s\n", dir == FORWARDS ? "forwards" : "backwards");
    digitalWrite(MOTOR_RIGHT_DIR_PIN, dir);
  }
}

void updateDesiredRPM(int change){
  wheelRPM  += RPM_STEP_SIZE*change;
  wheelRPM = constrain(wheelRPM, 0, MAX_WHEEL_RPM);
  Serial.printf("Changing speed by %d\n New wheel RPM is %d\n", change, wheelRPM);

  desiredRPM = wheelRPM * GEAR_RATIO;
  desiredRPM = constrain(desiredRPM, 0, MAX_ENC_RPM);
  Serial.printf("New encoder RPM is %d\n", desiredRPM);
}

void matchDesiredRPM(){
  for(int i = 0; i<MOTOR_COUNT; i++){//CHANGE TO MOTOR COUNT




    motorActualRPM[i] = calculateRPM(i);
    if(desiredRPM == 0){
      motorOutputVoltage[i] = 0; //sets the speed to 0 if the desiredRPM is 0
    }
    else{
      float error = ((desiredRPM - motorActualRPM[i]) / desiredRPM);
      // error = constrain(error, -0.1, 0.1); //Forces the motor to slowly change
      Serial.printf("The error for motor %d is %f\n", i, error);
      motorOutputVoltage[i] += error; //Adds the error the the output voltage
    }
    /* this logic was given by Frankie Sharman, it can be expanded upon in the future and should be a PID controller 
    It normalizes the error (the stuff on the right) and adds it to the actual (the error could be a positive or negative value)*/
    motorOutputVoltage[i] = constrain(motorOutputVoltage[i], 0, MAX_VOLTAGE); //Sets the output voltage to a minimum of 0 or a max of 3.3
    Serial.printf("The output voltage for motor %d is: %f\n", i, motorOutputVoltage[i]);
    analogWrite(motorPWMPin[i], motorOutputVoltage[i] * 77);
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
  Serial.printf("The period is %d\n", period);
  float freq = MICROS_PER_SECOND / period; //gets the frequency in Hz
  Serial.printf("The frequency is %f\n", freq);
  float rpm  = RPM_FROM_FREQ(freq); //calculates rpm
  Serial.printf("The calculated rpm for motor number %d is %f\n", motorNum, rpm);
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