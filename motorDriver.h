#ifndef MOTORDRIVER_H
#define MOTORDRIVER_H

#include <msp430fr2476.h>
#include "solenoid.h"
#include "TimerA.h"

#define X_MAX 254
#define Y_MAX 254

// X-Direction
//-----------------Motor 1---------------------------------------------------
#define M1_STEP_BIT BIT3
#define M1_STEP_DDR P3DIR
#define M1_STEP_PORT P3OUT
#define SET_M1_STEP_AS_AN_OUTPUT M1_STEP_DDR |= M1_STEP_BIT
#define M1_STEP_SET_HIGH M1_STEP_PORT |= M1_STEP_BIT
#define M1_STEP_SET_LOW M1_STEP_PORT &= ~M1_STEP_BIT
#define M1_STEP_TOGGLE M1_STEP_PORT ^= M1_STEP_BIT

#define M1_ENABLE_BIT BIT2
#define M1_ENABLE_DDR P3DIR
#define M1_ENABLE_PORT P3OUT
#define SET_M1_ENABLE_AS_OUTPUT M1_ENABLE_DDR |= M1_ENABLE_BIT
#define M1_ENABLE_HIGH M1_ENABLE_PORT |= M1_ENABLE_BIT //Disables outputs
#define M1_ENABLE_LOW M1_ENABLE_PORT &= ~M1_ENABLE_BIT //Enables outputs as required

#define M1_MS1_BIT BIT0
#define M1_MS1_DDR P3DIR
#define M1_MS1_PORT P3OUT
#define SET_M1_MS1_AS_OUTPUT M1_MS1_DDR |= M1_MS1_BIT
#define M1_MS1_FULL_STEP M1_MS1_PORT &= ~M1_MS1_BIT
#define M1_MS1_HALF_STEP M1_MS1_PORT |= M1_MS1_BIT

#define M1_RESET_BIT BIT1
#define M1_RESET_DDR P3DIR
#define M1_RESET_PORT P3OUT
#define SET_M1_RESET_AS_AN_OUTPUT M1_RESET_DDR |= M1_RESET_BIT
#define M1_RESET_SET_HIGH M1_RESET_PORT |= M1_RESET_BIT
#define M1_RESET_SET_LOW M1_RESET_PORT &= ~M1_RESET_BIT

#define M1_DIR_BIT BIT4
#define M1_DIR_DDR P3DIR
#define M1_DIR_PORT P3OUT
#define SET_M1_DIR_AS_OUTPUT M1_DIR_DDR |= M1_DIR_BIT
#define M1_DIR_FORWARD M1_DIR_PORT |= M1_DIR_BIT //CCW
#define M1_DIR_BACKWARD M1_DIR_PORT &= ~M1_DIR_BIT //CW

// Y-Direction
//-----------------Motor 2---------------------------------------------------
#define M2_STEP_BIT BIT3
#define M2_STEP_DDR P4DIR
#define M2_STEP_PORT P4OUT
#define SET_M2_STEP_AS_AN_OUTPUT M2_STEP_DDR |= M2_STEP_BIT
#define M2_STEP_SET_HIGH M2_STEP_PORT |= M2_STEP_BIT
#define M2_STEP_SET_LOW M2_STEP_PORT &= ~M2_STEP_BIT
#define M2_STEP_TOGGLE M2_STEP_PORT ^= M2_STEP_BIT

#define M2_ENABLE_BIT BIT2
#define M2_ENABLE_DDR P4DIR
#define M2_ENABLE_PORT P4OUT
#define SET_M2_ENABLE_AS_OUTPUT M2_ENABLE_DDR |= M2_ENABLE_BIT
#define M2_ENABLE_HIGH M2_ENABLE_PORT |= M2_ENABLE_BIT //Disables outputs
#define M2_ENABLE_LOW M2_ENABLE_PORT &= ~M2_ENABLE_BIT //Enables outputs as required

#define M2_MS1_BIT BIT0
#define M2_MS1_DDR P4DIR
#define M2_MS1_PORT P4OUT
#define SET_M2_MS1_AS_OUTPUT M2_MS1_DDR |= M2_MS1_BIT
#define M2_MS1_FULL_STEP M2_MS1_PORT &= ~M2_MS1_BIT
#define M2_MS1_HALF_STEP M2_MS1_PORT |= M2_MS1_BIT

#define M2_RESET_BIT BIT1
#define M2_RESET_DDR P4DIR
#define M2_RESET_PORT P4OUT
#define SET_M2_RESET_AS_AN_OUTPUT M2_RESET_DDR |= M2_RESET_BIT //CONFUSED BY THIS. SAYS IT SHOULD BE HIGH before step
#define M2_RESET_SET_HIGH M2_RESET_PORT |= M2_RESET_BIT
#define M2_RESET_SET_LOW M2_RESET_PORT &= ~M2_RESET_BIT

#define M2_DIR_BIT BIT4
#define M2_DIR_DDR P4DIR
#define M2_DIR_PORT P4OUT
#define SET_M2_DIR_AS_OUTPUT M2_DIR_DDR |= M2_DIR_BIT
#define M2_DIR_BACKWARD M2_DIR_PORT |= M2_DIR_BIT //CCW - backward
#define M2_DIR_FORWARD M2_DIR_PORT &= ~M2_DIR_BIT //CW - forward

//Prototypes
void InitializeMotorDriver(void);
void moveTo(unsigned int x, unsigned int y);
void moveToX(int x);
void moveToY(int y);
void highlightWord(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2); //Maybe a boolean
void setXMotorForward(void);
void setYMotorForward(void);
void setXMotorBackward(void);
void setYMotorBackward(void);

#endif
