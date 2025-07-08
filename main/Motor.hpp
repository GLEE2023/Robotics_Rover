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

#define MOTOR_ONE            0
#define MOTOR_TWO            1
#define MOTOR_THREE          2
#define MOTOR_FOUR           3

static uint8_t desiredSpeed;

volatile uint32_t motorActualSpeed[MOTOR_COUNT];
volatile uint32_t motorOneActualSpeed;
volatile uint32_t motorTwoActualSpeed;
volatile uint32_t motorThreeActualSpeed;
volatile uint32_t motorFourActualSpeed;

void motorInit();

void updateDesiredSpeed(uint8_t newSpeed);

void matchDesiredSpeed

#endif