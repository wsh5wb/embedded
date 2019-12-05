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
unsigned int x1 = 0, x2 = 0, y1 = 0, y2 = 0;

void moveToOrigin(void){
    PowerSolenoid();
    setXMotorBackward();
    setYMotorBackward();
    M1_ENABLE_LOW;
    M2_ENABLE_LOW;
    //check that the motors match the limit switch
    while(readLS1()){
        M1_STEP_SET_LOW;
        delay(10500); //10500
        M1_STEP_SET_HIGH;
        delay(10500);
    }while(readLS2()){
        M2_STEP_SET_LOW;
        delay(10500);
        M2_STEP_SET_HIGH;
        delay(10500);
    }

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
	InitializeSolenoid();
	InitializeUART();
	InitializeLimitSwitches();

	moveToOrigin(); //return to Origin
	//moveTo(20,30);
	//highlightWord(0,0,20,20);
	//UARTSendByte(0x64);
	while(1){
	    //UARTSendByte(0x64);
	    //UARTSendByte(0xff);

/*
	    if(System_Active){

	        while(System_Active){
	            if(Word_Ready == 1){
	                Word_Ready = 0;
	                highlightWord(x1,y1,x2,y2);
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
