#include <msp430.h>
#include "go.h"
#include "LED.h"
#include "solenoid.h"
#include "UART.h"
#include "TimerA.h"
#include "motorDriver.h"
#include "limitSwitch.h"

#define XIN_BIT BIT1
#define XOUT_BIT BIT0

//Global Variable
int System_Active = 0; // 0-OFF, 1-ON
int Word_Ready = 0;
unsigned int xPt = 0, yPt = 0, xDest = 0, yDest = 0;
unsigned int wordX1 = 0, wordY1 = 0, wordX2 = 0, wordY2 = 0;

void moveToOrigin(void){
    PowerSolenoid();
    setXMotorBackward();
    setYMotorBackward();
    M1_ENABLE_LOW;
    M2_ENABLE_LOW;
    //check that the motors match the limit switch
    while(readLS1()){
        M1_STEP_SET_LOW;
        delay(2500); //10500
        M1_STEP_SET_HIGH;
        delay(2500);
    }
    xPt = 0;
    while(readLS2()){
        M2_STEP_SET_LOW;
        delay(2500);
        M2_STEP_SET_HIGH;
        delay(2500);
    }
    yPt = 0;

    M1_ENABLE_HIGH;
    M2_ENABLE_HIGH;
    M1_RESET_SET_LOW;
    M2_RESET_SET_LOW;

    TurnOffSolenoid();
}

/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

    PM5CTL0 &= ~LOCKLPM5;

	//------------------------------- Configure the Clock -----------------------------------

	P2SEL0 |= XIN_BIT + XOUT_BIT;
	//CSCTL6 |= XT1BYPASS; //XT1 bypass select that allows external crystal
	CSCTL4 |= SELMS_0; // Sets DCOCLKDIV as the source for MClk
	CSCTL1 = DCORSEL_3; //Sets DC0 to 1 MHz

	CSCTL2 = FLLD_0; //Set FLLD to 0
	CSCTL2 |= 0x00FF; //Set FLLN to 255
	CSCTL3 &= ~SELREF; //Set FLL reference as XT1Clk
	CSCTL3 &= ~FLLREFDIV; //Set FLLREFDIV to zero

	ConfigureTimerA();

	//------------------------------ Initialize Pins -------------------------------------------
	_enable_interrupts();
	InitializeLEDPortPin();
	InitializeGoButtonPortPin();
	InitializeMotorDriver();
	InitializeLimitSwitches();
	InitializeSolenoid();
	InitializeUART();


	moveToOrigin(); //return to Origin
	//PowerSolenoid();
	//moveTo(400,0);
	highlightWord(0,0,15,16);
	//moveToOrigin();
	//UARTSendByte(0x64);
	while(1){
	    /*if(!readLS1()){
	        TurnLEDOff();
	    }
	    if(!readLS2()){
	        TurnLEDOn();
	    }*/
	    //UARTSendByte(0x64);
	    //UARTSendByte(0xff);

/*
	    if(System_Active){

	        while(System_Active){
	            if(Word_Ready == 1){
	                Word_Ready = 0;
	                highlightWord(wordX1,wordY1,wordX2,wordY2);
	            }
	        }
	        moveToOrigin();
	        System_Active = 0;
	        SET_LED_LOW;
	    }
*/
	}

	return 0;
}
