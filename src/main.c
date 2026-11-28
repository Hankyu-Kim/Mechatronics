#include <BOARD.h>
#include <xc.h>
#include <stdio.h>
#include <AD.h>
//#include "ES_Configure.h"
//#include "ES_Framework.h"
#include <sensor.h>
#include <RC_Servo.h>

// ==========================================
// Main
// ==========================================

int main(void)
{
    // ======================================
    // Initialization
    // ======================================

    BOARD_Init();
    PWM_Init();

    Motor_Init();
    Bumper_Sensor_Init();
    IR_Tape_Sensor_Init();
    
    // flywheel
    GPIO_Test_Init();
    
    // servo
    RC_Init();
    RC_AddPins(RC_PORTZ08);

    // ======================================
    // Test
    // ======================================

    while (1)
    {
//        // Motor Test
//        Motor_Control(FRONT_DRIVE, 750);
//
//        // Bumper Test
//        printf("Bumper Left : %d\r\n",
//               Read_Bumper(BUMPER_LEFT));
//
//        printf("Bumper Right: %d\r\n",
//               Read_Bumper(BUMPER_RIGHT));
//
//        // IR Tape Sensor Test (PORTV)
//        printf("IR Front  : %d\r\n",
//               Read_IR_Tape_Sensor(IR_FRONT));
//
//        printf("IR Left   : %d\r\n",
//               Read_IR_Tape_Sensor(IR_LEFT));
//
//        printf("IR Center : %d\r\n",
//               Read_IR_Tape_Sensor(IR_CENTER));
//
//        printf("IR Right  : %d\r\n",
//               Read_IR_Tape_Sensor(IR_RIGHT));
//
//        // IR Tape Sensor Test (PORTW)
//        printf("IR Back   : %d\r\n",
//               Read_IR_Tape_Sensor(IR_BACK));
//
//        printf("IR Side 1 : %d\r\n",
//               Read_IR_Tape_Sensor(IR_SIDE_ONE));
//
//        printf("IR Side 2 : %d\r\n",
//               Read_IR_Tape_Sensor(IR_SIDE_TWO));
//
//        printf("IR Side 3 : %d\r\n",
//               Read_IR_Tape_Sensor(IR_SIDE_THREE));
        
        // Fly wheel test
        // PWM_SetDutyCycle(PWM_PORTZ06, 1000);
        
        // Servo test
        RC_SetPulseTime(RC_PORTZ08, 500);

        for (volatile int i = 1000; i < 2000; i+=50) {

        RC_SetPulseTime(RC_PORTZ08, i);
        
        printf("test");
        for (volatile int i = 0; i < 500000; i++);
        }

        // Delay
        //for (volatile int i = 0; i < 500000; i++);
    }

    return 0;
}