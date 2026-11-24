
#include "sensor.h"

int bumper_threshold = 1000;

// ==========================================
// Initialization
// ==========================================

void Motor_Init(void)
{
    PWM_SetFrequency(2000);
    PWM_AddPins(LEFT_PWM | RIGHT_PWM);

    IO_PortsSetPortOutputs(
        MOTOR_PORT,
        IN1 | IN2 | IN3 | IN4
    );

    Motor_Control(STOP, 0);
}

void Bumper_Sensor_Init(void)
{
    AD_Init();
    AD_AddPins(LEFT_BUMPER | RIGHT_BUMPER);
}

void IR_Tape_Sensor_Init(void)
{
    IO_PortsSetPortInputs(
        FRONT_IR_PORT,
        FRONT_IR | LEFT_IR | CENTER_IR | RIGHT_IR
    );

    IO_PortsSetPortInputs(
        BACK_IR_PORT,
        BACK_IR | SIDE_ONE_IR | SIDE_TWO_IR | SIDE_THREE_IR
    );
}

// ==========================================
// Motor Control
// ==========================================

#define LEFT_MOTOR_SCALE  (7.15f / 8.0f)
#define DIFFERENTIAL_SCALE 0.8f

void Motor_Control(MotorState state, unsigned int motor_speed)
{
    unsigned int left_speed = motor_speed;
    unsigned int right_speed = motor_speed;

    switch (state)
    {
        case FORWARD_DRIVE:
            IO_PortsWritePort(MOTOR_PORT, IN1 | IN3);
            break;

        case REAR_DRIVE:
            IO_PortsWritePort(MOTOR_PORT, IN2 | IN4);
            break;

        case TURN_LEFT:
            IO_PortsWritePort(MOTOR_PORT, IN2 | IN3);
            break;

        case TURN_RIGHT:
            IO_PortsWritePort(MOTOR_PORT, IN1 | IN4);
            break;

        case DIFFERENTIAL_LEFT_DRIVE:
            IO_PortsWritePort(MOTOR_PORT, IN1 | IN3);
            left_speed = motor_speed * DIFFERENTIAL_SCALE;
            break;

        case DIFFERENTIAL_RIGHT_DRIVE:
            IO_PortsWritePort(MOTOR_PORT, IN1 | IN3);
            right_speed = motor_speed * DIFFERENTIAL_SCALE;
            break;

        case STOP:
        default:
            PWM_SetDutyCycle(LEFT_PWM, 0);
            PWM_SetDutyCycle(RIGHT_PWM, 0);
            IO_PortsWritePort(MOTOR_PORT, 0);
            return;
    }

    // Left motor calibration
    left_speed = left_speed * LEFT_MOTOR_SCALE;

    // Apply PWM duty cycles
    PWM_SetDutyCycle(LEFT_PWM, left_speed);
    PWM_SetDutyCycle(RIGHT_PWM, right_speed);
}

// ==========================================
// Bumper Sensor
// ==========================================

bool Read_Bumper(BumperEnum bumper_enum)
{
    unsigned int read_pin;

    if (bumper_enum == BUMPER_LEFT)
        read_pin = LEFT_BUMPER;
    else if (bumper_enum == BUMPER_RIGHT)
        read_pin = RIGHT_BUMPER;
    else
        return false;

    int value = AD_ReadADPin(read_pin);

    return (value > bumper_threshold);
}

// ==========================================
// IR Tape Sensor
// ==========================================

bool Read_IR_Tape_Sensor(IrTapeSensorEnum ir_enum)
{
    unsigned int read_port;
    unsigned int read_pin;

    switch (ir_enum)
    {
        case IR_FRONT:
            read_port = FRONT_IR_PORT;
            read_pin = FRONT_IR;
            break;

        case IR_LEFT:
            read_port = FRONT_IR_PORT;
            read_pin = LEFT_IR;
            break;

        case IR_CENTER:
            read_port = FRONT_IR_PORT;
            read_pin = CENTER_IR;
            break;

        case IR_RIGHT:
            read_port = FRONT_IR_PORT;
            read_pin = RIGHT_IR;
            break;

        case IR_BACK:
            read_port = BACK_IR_PORT;
            read_pin = BACK_IR;
            break;

        case IR_SIDE_ONE:
            read_port = BACK_IR_PORT;
            read_pin = SIDE_ONE_IR;
            break;

        case IR_SIDE_TWO:
            read_port = BACK_IR_PORT;
            read_pin = SIDE_TWO_IR;
            break;

        case IR_SIDE_THREE:
            read_port = BACK_IR_PORT;
            read_pin = SIDE_THREE_IR;
            break;

        default:
            return false;
    }

    unsigned int data = IO_PortsReadPort(read_port);

    return ((data & read_pin) != 0);
}
