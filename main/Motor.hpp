#ifndef MOTOR_H
#define MOTOR_H

#define MOTOR_ONE_PWM_PIN   21
#define MOTOR_TWO_PWM_PIN   19
#define MOTOR_THREE_PWM_PIN 18
#define MOTOR_FOUR_PWM_PIN  5

#define MOTOR_ONE_ENC_PIN   32
#define MOTOR_TWO_ENC_PIN   33
#define MOTOR_THREE_ENC_PIN 23
#define MOTOR_FOUR_ENC_PIN  25

#define MOTOR_LEFT_DIR_PIN   22
#define MOTOR_RIGHT_DIR_PIN  23

#define MOTOR_COUNT          4
#define PULSE_PER_REVOLUTION 24

#define MOTOR_ONE            0
#define MOTOR_TWO            1
#define MOTOR_THREE          2
#define MOTOR_FOUR           3



static uint32_t desiredSpeed;

volatile uin32_t motorPulsePeriod[MOTOR_COUNT] = {0};
volatile uint32_t motorLastPulseTime[MOTOR_COUNT] = {0};

volatile uint32_t motorActualRPM[MOTOR_COUNT] = {0};


void motorInit();

void updateDesiredRPM(uint32_t newSpeed); //Updates the global desired speed for each motor (this happens through the controller from ESP-NOW)

void matchDesiredSpeed(uint32_t )

// void getActualSpeed(uint32_t motorNUM);

uint32_t getPulsePeriod(uint32_t motorNUM); //Disables interrupts and returns the period, this is a safer way to get the pulses rather than just reading from the global variable

void IRAM_ATTR motorOneEncoderISR();
void IRAM_ATTR motorTwoEncoderISR();
void IRAM_ATTR motorThreeEncoderISR();
void IRAM_ATTR motorFourEncoderISR();

#endif