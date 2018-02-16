/*
open-dobot firmware.

Find driver and SDK at https://github.com/maxosprojects/open-dobot

Author: maxosprojects (March 18 2016)
Additional Authors: <put your name here>

Version: 1.2.2

License: MIT
*/

/**
RAMPS version routines.
**/

#include <avr/interrupt.h>
#include "../main/dobot.h"
#include "../main/misc/calibrator.h"
#include "../main/misc/queue.h"

#define X_STEP_PIN         PORTB3
#define X_STEP_PORT        PORTB
#define X_STEP_DDR         DDRB

#define X_DIR_PIN          PORTB4
#define X_DIR_PORT         PORTB
#define X_DIR_DDR          DDRB

#define X_ENABLE_PIN       PORTB2
#define X_ENABLE_PORT      PORTB
#define X_ENABLE_DDR       DDRB

#define Y_STEP_PIN         PORTB0
#define Y_STEP_PORT        PORTB
#define Y_STEP_DDR         DDRB

#define Y_DIR_PIN          PORTB1
#define Y_DIR_PORT         PORTB
#define Y_DIR_DDR          DDRB

#define Y_ENABLE_PIN       PORTD7
#define Y_ENABLE_PORT      PORTD
#define Y_ENABLE_DDR       DDRD

// #define Z_STEP_PIN         PORTL3
// #define Z_STEP_PORT        PORTL
// #define Z_STEP_DDR         DDRL
// #define Z_DIR_PIN          PORTL1
// #define Z_ENABLE_PIN       PORTK0

// Use E0 instead of Z as it is not easy to connect dobot to Z.
// Left named as Z to avoif renaming.
#define Z_STEP_PIN         PORTD5
#define Z_STEP_PORT        PORTD
#define Z_STEP_DDR         DDRD

#define Z_DIR_PIN          PORTD6
#define Z_DIR_PORT         PORTD
#define Z_DIR_DDR          DDRD

#define Z_ENABLE_PIN       PORTD4
#define Z_ENABLE_PORT      PORTD
#define Z_ENABLE_DDR       DDRD

#define LASER_PIN PORTD3
#define LASER_PORT PORTD
#define LASER_DDR DDRD

#define PUMP_PIN PORTD2
#define PUMP_PORT PORTD
#define PUMP_DDR DDRD

#define VALVE_PIN PORTB5
#define VALVE_PORT PORTB
#define VALVE_DDR DDRB

#define TOOL_ROT_PIN 0 //PORTH3
#define TOOL_ROT_DDR 0 //DDRH
#define TOOL_ROT_PWM 0 //OCR4A

#define GRIPPER_PIN 0 //PORTE3
#define GRIPPER_DDR 0 //DDRE
#define GRIPPER_PWM 0 //OCR3A

// At 50kHz how many ticks pass between TIMER5_COMPA_vect ISR calls.
#define TICKS_PER_CALL 40
// Coefficient that is used in DobotDriver to calculate stepping
// period.
#define STEPS_COEFF 20000
// How long delay does the TIMER1_COMPA_vect ISR introduce.
// This is to be acoounted for make corresponding adjustments.
#define ISR_DELAY 80

extern CommandQueue cmdQueue;
extern Calibrator calibrator;
