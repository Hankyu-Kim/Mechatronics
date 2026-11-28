/*
 * File:   TemplateEventChecker.c
 * Author: Gabriel Hugh Elkaim
 *
 * Template file to set up typical EventCheckers for the  Events and Services
 * Framework (ES_Framework) on the Uno32 for the CMPE-118/L class. Note that
 * this file will need to be modified to fit your exact needs, and most of the
 * names will have to be changed to match your code.
 *
 * This EventCheckers file will work with both FSM's and HSM's.
 *
 * Remember that EventCheckers should only return TRUE when an event has occured,
 * and that the event is a TRANSITION between two detectable differences. They
 * should also be atomic and run as fast as possible for good results.
 *
 * This file includes a test harness that will run the event detectors listed in the
 * ES_Configure file in the project, and will conditionally compile main if the macro
 * EVENTCHECKER_TEST is defined (either in the project or in the file). This will allow
 * you to check you event detectors in their own project, and then leave them untouched
 * for your project unless you need to alter their post functions.
 *
 * Created on September 27, 2013, 8:37 AM
 */

/*******************************************************************************
 * MODULE #INCLUDE                                                             *
 ******************************************************************************/

#include "ES_Configure.h"
#include "TemplateEventChecker.h"
#include "ES_Events.h"
#include "serial.h"
#include "AD.h"
#include <stdio.h>
#include "sensor.h"

/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/
//#define BATTERY_DISCONNECT_THRESHOLD 175

/*******************************************************************************
 * EVENTCHECKER_TEST SPECIFIC CODE                                                             *
 ******************************************************************************/

//#define EVENTCHECKER_TEST
#ifdef EVENTCHECKER_TEST
#include <stdio.h>
#define SaveEvent(x) do {eventName=__func__; storedEvent=x;} while (0)

static const char *eventName;
static ES_Event storedEvent;
#endif

/*******************************************************************************
 * PRIVATE FUNCTION PROTOTYPES                                                 *
 ******************************************************************************/
/* Prototypes for private functions for this EventChecker. They should be functions
   relevant to the behavior of this particular event checker */

/*******************************************************************************
 * PRIVATE MODULE VARIABLES                                                    *
 ******************************************************************************/

/* Any private module level variable that you might need for keeping track of
   events would be placed here. Private variables should be STATIC so that they
   are limited in scope to this module. */

/*******************************************************************************
 * PUBLIC FUNCTIONS                                                            *
 ******************************************************************************/

/**
 * @Function TemplateCheckBattery(void)
 * @param none
 * @return TRUE or FALSE
 * @brief This function is a prototype event checker that checks the battery voltage
 *        against a fixed threshold (#defined in the .c file). Note that you need to
 *        keep track of previous history, and that the actual battery voltage is checked
 *        only once at the beginning of the function. The function will post an event
 *        of either BATTERY_CONNECTED or BATTERY_DISCONNECTED if the power switch is turned
 *        on or off with the USB cord plugged into the Uno32. Returns TRUE if there was an 
 *        event, FALSE otherwise.
 * @note Use this code as a template for your other event checkers, and modify as necessary.
 * @author Gabriel H Elkaim, 2013.09.27 09:18
 * @modified Gabriel H Elkaim/Max Dunne, 2016.09.12 20:08 */
// uint8_t TemplateCheckBattery(void) {
//     static ES_EventTyp_t lastEvent = BATTERY_DISCONNECTED;
//     ES_EventTyp_t curEvent;
//     ES_Event thisEvent;
//     uint8_t returnVal = FALSE;
//     uint16_t batVoltage = AD_ReadADPin(BAT_VOLTAGE); // read the battery voltage
    
// //    printf("TemplateCheckBattery works");

//     if (batVoltage > BATTERY_DISCONNECT_THRESHOLD) { // is battery connected?
//         curEvent = BATTERY_CONNECTED;
//     } else {
//         curEvent = BATTERY_DISCONNECTED;
//     }
//     if (curEvent != lastEvent) { // check for change from last time
//         thisEvent.EventType = curEvent;
//         thisEvent.EventParam = batVoltage;
//         returnVal = TRUE;
//         lastEvent = curEvent; // update history
// #ifndef EVENTCHECKER_TEST           // keep this as is for test harness
//         PostTemplateFSM(thisEvent);
// #else
//         SaveEvent(thisEvent);
// #endif   
//     }
//     return (returnVal);
// }

#define IR_DEBOUNCE_MS 30

uint8_t CheckIRPattern(void)
{
    static uint16_t last_raw_pattern = 0;
    static uint16_t stable_pattern = 0;
    static uint32_t last_change_time = 0;
    static uint8_t initialized = FALSE;

    uint16_t pattern = 0;
    uint32_t current_time = ES_Timer_GetTime();

    uint8_t returnVal = FALSE;

    // Read all 8 IR sensors
    pattern = Read_IR_Tape_Sensor(IR_FRONT_LEFT) << 7;
    pattern |= Read_IR_Tape_Sensor(IR_FRONT) << 6;
    pattern |= Read_IR_Tape_Sensor(IR_FRONT_RIGHT) << 5;
    pattern |= Read_IR_Tape_Sensor(IR_CENTER) << 4;
    pattern |= Read_IR_Tape_Sensor(IR_RIGHT) << 3;
    pattern |= Read_IR_Tape_Sensor(IR_BACK) << 2;
//    pattern |= Read_IR_Tape_Sensor(IR_SIDE_TWO) << 1;
//    pattern |= Read_IR_Tape_Sensor(IR_SIDE_THREE);
    
//    printf("IR Pattern: %u %u %u %u %u %u %u %u\r\n",
//   (pattern >> 7) & 1,
//   (pattern >> 6) & 1,
//   (pattern >> 5) & 1, // tape(dark) is 1, field(white) is 0
//   (pattern >> 4) & 1,
//   (pattern >> 3) & 1,
//   (pattern >> 2) & 1,
//   (pattern >> 1) & 1,
//   (pattern >> 0) & 1);

    // Initialize without generating an event
    if (initialized == FALSE) {
        last_raw_pattern = pattern;
        stable_pattern = pattern;
        last_change_time = current_time;
        initialized = TRUE;
        return FALSE;
    }

    // Detect raw pattern changes
    if (pattern != last_raw_pattern) {
        last_raw_pattern = pattern;
        last_change_time = current_time;
    }

    // Generate event only after pattern is stable
    if ((uint32_t)(current_time - last_change_time) >= IR_DEBOUNCE_MS) {

        if (pattern != stable_pattern) {
            ES_Event thisEvent;

            stable_pattern = pattern;

            thisEvent.EventType = CHECK_IR_PATTERN;
            thisEvent.EventParam = stable_pattern;

            returnVal = PostTemplateFSM(thisEvent);
        }
    }

    return returnVal;
}

/* 
 * The Test Harness for the event checkers is conditionally compiled using
 * the EVENTCHECKER_TEST macro (defined either in the file or at the project level).
 * No other main() can exist within the project.
 * 
 * It requires a valid ES_Configure.h file in the project with the correct events in 
 * the enum, and the correct list of event checkers in the EVENT_CHECK_LIST.
 * 
 * The test harness will run each of your event detectors identically to the way the
 * ES_Framework will call them, and if an event is detected it will print out the function
 * name, event, and event parameter. Use this to test your event checking code and
 * ensure that it is fully functional.
 * 
 * If you are locking up the output, most likely you are generating too many events.
 * Remember that events are detectable changes, not a state in itself.
 * 
 * Once you have fully tested your event checking code, you can leave it in its own
 * project and point to it from your other projects. If the EVENTCHECKER_TEST marco is
 * defined in the project, no changes are necessary for your event checkers to work
 * with your other projects.
 */
//#ifdef EVENTCHECKER_TEST
//#include <stdio.h>
//static uint8_t(*EventList[])(void) = {EVENT_CHECK_LIST};
//
//void PrintEvent(void);
//
//void main(void) {
//    BOARD_Init();
//    /* user initialization code goes here */
//
//    // Do not alter anything below this line
//    int i;
//
//    printf("\r\nEvent checking test harness for %s", __FILE__);
//
//    while (1) {
//        if (IsTransmitEmpty()) {
//            for (i = 0; i< sizeof (EventList) >> 2; i++) {
//                if (EventList[i]() == TRUE) {
//                    PrintEvent();
//                    break;
//                }
//
//            }
//        }
//    }
//}
//
//void PrintEvent(void) {
//    printf("\r\nFunc: %s\tEvent: %s\tParam: 0x%X", eventName,
//            EventNames[storedEvent.EventType], storedEvent.EventParam);
//}
//#endif