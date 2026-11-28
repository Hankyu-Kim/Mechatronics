
#ifndef SENSOR_H
#define SENSOR_H

#include <stdbool.h>
#include "pwm.h"
#include "IO_Ports.h"
#include "AD.h"

// PWM Pins
#define LEFT_PWM    PWM_PORTY10
#define RIGHT_PWM   PWM_PORTY12

// Motor Pins
#define MOTOR_PORT  PORTX

#define IN1 PIN3
#define IN2 PIN4
#define IN3 PIN5
#define IN4 PIN6

// Bumper Analog Pins
#define LEFT_BUMPER  AD_PORTV3
#define RIGHT_BUMPER AD_PORTV4

// IR Tape Sensor Ports
#define FRONT_IR_PORT PORTV
#define BACK_IR_PORT  PORTW

// PORTV
#define FRONT_LEFT_IR  PIN5
#define FRONT_IR   PIN6
#define FRONT_RIGHT_IR PIN7
#define CENTER_IR   PIN8

// PORTW
#define RIGHT_IR       PIN3
#define BACK_IR   PIN4
#define SIDE_TWO_IR   PIN5
#define SIDE_THREE_IR PIN6

// Shooting Pins
#define FLYWHEEL_PORT PORTZ
#define FLYWHEEL_PIN  PIN3

#define SERVO_PORT    PORTZ
#define SERVO_PIN     PIN4

// Servo Parameters
#define SERVO_PERIOD_US  20000
#define SERVO_MIN_US      1000
#define SERVO_CENTER_US   1500
#define SERVO_MAX_US      2000

// Motor States
typedef enum {
    STOP,
    FORWARD_DRIVE,
    REAR_DRIVE,
    TURN_LEFT,
    TURN_RIGHT,
    DIFFERENTIAL_LEFT_DRIVE,
    DIFFERENTIAL_RIGHT_DRIVE
} MotorState;

// Bumper States
typedef enum {
    BUMPER_LEFT,
    BUMPER_RIGHT
} BumperEnum;

// IR Tape Sensor States
typedef enum {
    IR_FRONT_LEFT,
    IR_FRONT,
    IR_FRONT_RIGHT,
    IR_CENTER,
    IR_RIGHT,
    IR_BACK,
    IR_SIDE_TWO,
    IR_SIDE_THREE
} IrTapeSensorEnum;

// Initialization
void Motor_Init(void);
void Bumper_Sensor_Init(void);
void IR_Tape_Sensor_Init(void);

// Motor Control
void Motor_Control(MotorState state, unsigned int motor_speed);

// Sensor Reading
bool Read_Bumper(BumperEnum bumper_enum);
bool Read_IR_Tape_Sensor(IrTapeSensorEnum ir_enum);

void GPIO_Test_Init();
void GPIO_SetHigh();
void GPIO_SetLow();

#endif
