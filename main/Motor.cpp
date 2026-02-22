#include "Motor.hpp"

  const uint8_t motorPWMPin[MOTOR_COUNT] = {MOTOR_FR_PWM_PIN, MOTOR_BR_PWM_PIN, MOTOR_FL_PWM_PIN, MOTOR_BL_PWM_PIN};

  volatile uint32_t motorPulsePeriod[MOTOR_COUNT] = {0}; //Time between last encoder pulse and current encoder plse
  volatile uint32_t motorLastPulseTime[MOTOR_COUNT] = {0}; 

  volatile int desiredRPMLeft  = 0;
  volatile int desiredRPMRight = 0; 
  static int wheelRPMLeft      = 0;
  static int wheelRPMRight     = 0;

  float motorActualRPM[MOTOR_COUNT] = {0}; //Actual RPM calculated from the encoder pulses
  float motorOutputVoltage[MOTOR_COUNT] = {0}; //Outputted PWM to get the desired RPM

  void motorInit(){

    /* PWM Pins Init */
    pinMode(MOTOR_FR_PWM_PIN, OUTPUT);
    pinMode(MOTOR_BR_PWM_PIN, OUTPUT);
    pinMode(MOTOR_FL_PWM_PIN, OUTPUT);
    pinMode(MOTOR_BL_PWM_PIN, OUTPUT);

    /* ENC PINS INIT */
    pinMode(MOTOR_FR_ENC_PIN, INPUT_PULLUP); /*NE12 datasheet says it is open-collect ref: https://www.servocity.com/content/downloads/ne12_-_use_parameter.pdf */
    pinMode(MOTOR_BR_ENC_PIN, INPUT_PULLUP);
    pinMode(MOTOR_FL_ENC_PIN, INPUT_PULLUP);
    pinMode(MOTOR_BL_ENC_PIN, INPUT_PULLUP);

    /* Direction Controls Init*/
    pinMode(MOTOR_LEFT_DIR_PIN, OUTPUT);
    pinMode(MOTOR_RIGHT_DIR_PIN, OUTPUT);

    /* Creates ISR for encoders */
    attachInterrupt(digitalPinToInterrupt(MOTOR_FR_ENC_PIN), motorFREncoderISR, RISING);
    attachInterrupt(digitalPinToInterrupt(MOTOR_BR_ENC_PIN), motorBREncoderISR, RISING);
    attachInterrupt(digitalPinToInterrupt(MOTOR_FL_ENC_PIN), motorFLEncoderISR, RISING);
    attachInterrupt(digitalPinToInterrupt(MOTOR_BL_ENC_PIN), motorBLEncoderISR, RISING);
  }

  /* Wrapper functions for motor drive direction */
  void motorDriveLeft(uint8_t dir){
    motorDrive(LEFT_SIDE, dir);
  }

  void motorDriveRight(uint8_t dir){
    motorDrive(RIGHT_SIDE, dir);
  }

  void motorDrive(uint8_t side, uint8_t dir){
    if(side == LEFT_SIDE){
      // Serial.printf("Left side is attempting to %s\n", dir == FORWARDS ? "forwards" : "backwards");
      digitalWrite(MOTOR_LEFT_DIR_PIN, dir);
    }
    else{ /* side = RIGHT_SIDE*/
      // Serial.printf("Right side is attempting to %s\n", dir == FORWARDS ? "forwards" : "backwards");
      digitalWrite(MOTOR_RIGHT_DIR_PIN, dir);
    }
  }

  /* Wrapper functions for updateDesiredRPM */
  void updateDesiredRPMLeft(int change){
    updateDesiredRPM(LEFT_SIDE, change);
  }

  void updateDesiredRPMRight(int change){
    updateDesiredRPM(RIGHT_SIDE, change);
  }

  void updateDesiredRPM(uint8_t side, int change){
    if(side == LEFT_SIDE){
      wheelRPMLeft  += RPM_STEP_SIZE*change;
      wheelRPMLeft = constrain(wheelRPMLeft, 0, MAX_WHEEL_RPM);

      //DEBUG
      //Serial.printf("Changing left side speed by %d\n New wheel RPM is %d\n", change, wheelRPMLeft);

      desiredRPMLeft = wheelRPMLeft * GEAR_RATIO;
      desiredRPMLeft = constrain(desiredRPMLeft, 0, MAX_ENC_RPM);

      // DEBUG
      Serial.printf("New encoder RPM is %d\n", desiredRPMLeft);
    }
    else{
      wheelRPMRight  += RPM_STEP_SIZE*change;
      wheelRPMRight = constrain(wheelRPMRight, 0, MAX_WHEEL_RPM);

      //DEBUG
      //Serial.printf("Changing left side speed by %d\n New wheel RPM is %d\n", change, wheelRPMLeft);

      desiredRPMRight = wheelRPMRight * GEAR_RATIO;
      desiredRPMRight = constrain(desiredRPMRight, 0, MAX_ENC_RPM);

      // DEBUG
      Serial.printf("New encoder RPM is %d\n", desiredRPMRight);
    }

  }

  int getDesiredRPM(uint8_t index){
    if(index == MOTOR_FR || index == MOTOR_BR){
      return desiredRPMRight;
    }
    else{
      return desiredRPMLeft;
    }
  }

  void setDesiredRPM(uint8_t side, int rpm){
    if(side == LEFT_SIDE){
      desiredRPMLeft = rpm * GEAR_RATIO;
      desiredRPMLeft = constrain(desiredRPMLeft, 0, MAX_ENC_RPM);
    }
    else{
      desiredRPMRight = rpm * GEAR_RATIO;
      desiredRPMRight = constrain(desiredRPMRight, 0, MAX_ENC_RPM);
    }
  }

  void matchDesiredRPM(){

    for(int i = 0; i<MOTOR_COUNT; i++){
      int desiredRPM = getDesiredRPM(i);

      motorActualRPM[i] = calculateRPM(i);
      if(desiredRPM == 0){
        motorOutputVoltage[i] = 0; //sets the speed to 0 if the desiredRPM is 0
        analogWrite(motorPWMPin[i], 0); // can be removed later
        continue; // can be removed later or kept doesnt  matter when code returns to normal
      }
      else{ 
        float error = ((float)(desiredRPM - motorActualRPM[i]) / desiredRPM); //typecast for accurate float division
        error = constrain(error, -0.05, 0.05); //Forces the motor to slowly change

        //[DEBUG]
        // Serial.printf("The error for motor %d is %f\n", i, error);
        // Serial.printf("error:%f", error);
        
        motorOutputVoltage[i] += error; //Adds the error the the output voltage
      } 
        /* this logic was given by Frankie Sharman, it can be expanded upon in the future and should be a PID controller 
        It normalizes the error (the stuff on the right) and adds it to the actual (the error could be a positive or negative value)*/
        motorOutputVoltage[i] = constrain(motorOutputVoltage[i], 0.0, MAX_VOLTAGE); //Sets the output voltage to a minimum of 0 or a max of 3.3
        // Serial.printf("The output voltage for motor %d is: %f\n", i, motorOutputVoltage[i]);
        analogWrite(motorPWMPin[i], motorOutputVoltage[i] * 77); //We use 3.3V logic so *77 to scale to 255
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
      //DEBUG
      // Serial.println("Period could not be calculated");
      return 0; //motor startup so ignore this otherwise we have divide by 0
    }
    // Serial.printf("The period is %d\n", period);
    float freq = MICROS_PER_SECOND / period; //gets the frequency in Hz
    // Serial.printf("The frequency is %f\n", freq);
    float rpm  = RPM_FROM_FREQ(freq); //calculates rpm
    float wheel_rpm = WHEEL_RPM_FROM_ENC_RPM(rpm);
    Serial.printf("WheelRPM:%f\n", wheel_rpm);
    // Serial.printf("The calculated rpm for motor number %d is %f\n", motorNum, rpm);
    return rpm;
  }

  void rampDown(uint8_t side){
    if(side == RIGHT_SIDE){
      analogWrite(MOTOR_FR_PWM_PIN, 0);
      analogWrite(MOTOR_BR_PWM_PIN, 0);
    }
    else{
      analogWrite(MOTOR_FL_PWM_PIN, 0);
      analogWrite(MOTOR_BL_PWM_PIN, 0);
    }
  }

  void IRAM_ATTR motorFREncoderISR(){
    uint32_t currentTime = micros();
    motorPulsePeriod[MOTOR_FR] = currentTime - motorLastPulseTime[MOTOR_FR];
    motorLastPulseTime[MOTOR_FR] = currentTime;
  }

  void IRAM_ATTR motorBREncoderISR(){
    uint32_t currentTime = micros();
    motorPulsePeriod[MOTOR_BR] = currentTime - motorLastPulseTime[MOTOR_BR];
    motorLastPulseTime[MOTOR_BR] = currentTime;
  }

  void IRAM_ATTR motorFLEncoderISR(){
    uint32_t currentTime = micros();
    motorPulsePeriod[MOTOR_FL] = currentTime - motorLastPulseTime[MOTOR_FL];
    motorLastPulseTime[MOTOR_FL] = currentTime;
  }

  void IRAM_ATTR motorBLEncoderISR(){
    uint32_t currentTime = micros();
    motorPulsePeriod[MOTOR_BL] = currentTime - motorLastPulseTime[MOTOR_BL];
    motorLastPulseTime[MOTOR_BL] = currentTime;
  }