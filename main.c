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
int Word_Ready = 0; // 0 - Not ready, 1 - Ready to highlight word
unsigned int xPt = 0, yPt = 0, xDest = 0, yDest = 0;
unsigned int wordX1 = 0, wordY1 = 0, wordX2 = 0, wordY2 = 0; // Locations to be used for highlighting

/* moveToXAxis
 * Parameters - None
 * Return - None
 * Algorithm - Sets M1 to go backwards and steps on x until it closes LS1 meaning it's on the x-axis. It then updates xPt accordingly
 */
void moveToXAxis(void){
    setXMotorBackward();
    M1_ENABLE_LOW;
    while(readLS1() == 1){
        M1_STEP_SET_LOW;
        delay(2500); //10500
        M1_STEP_SET_HIGH;
        delay(2500);
    }
    xPt = 0;
    M1_ENABLE_HIGH;
    M1_RESET_SET_LOW;
}

/* moveToYAxis
 * Parameters - None
 * Return - None
 * Algorithm - Sets M2 to go backwards and steps on y until it closes LS2 meaning it's on the y-axis. It then updates yPt accordingly
 */
void moveToYAxis(void){
    setYMotorBackward();
    M2_ENABLE_LOW;
    while(readLS2() == 1){
        M2_STEP_SET_LOW;
        delay(2500);
        M2_STEP_SET_HIGH;
        delay(2500);
    }
    yPt = 0;
    M2_ENABLE_HIGH;
    M2_RESET_SET_LOW;
}

/* moveToOrigin
 * Parameters - None
 * Return - None
 * Algorithm - Picks up the pen and then calls moveToXAxis and moveToYAxis so that it ends up at the origin. It then drops the pen
 *             to stop running current through the solenoid.
 */
void moveToOrigin(void){
    PowerSolenoid();
    moveToXAxis();
    moveToYAxis();
    TurnOffSolenoid();
}

/* ConfigureClock
 * Parameters - None
 * Return - None
 * Algorithm - Sets up the external crystal and used DCOCLKDIV as source for master clock. It also sets up FLL to create a
 *             clock frequency of 8388608 Hz on DCOCLKDIV. Finally, it configures TimerA
 */
void ConfigureClock(void){
    P2SEL0 |= XIN_BIT + XOUT_BIT;
    CSCTL6 |= XT1BYPASS; //XT1 bypass select that allows external crystal
    CSCTL4 |= SELMS_0; // Sets DCOCLKDIV as the source for MClk
    CSCTL1 = DCORSEL_3; //Sets DC0 to 1 MHz

    CSCTL2 = FLLD_0; //Set FLLD to 0
    CSCTL2 |= 0x00FF; //Set FLLN to 255
    CSCTL3 &= ~SELREF; //Set FLL reference as XT1Clk
    CSCTL3 &= ~FLLREFDIV; //Set FLLREFDIV to zero

    ConfigureTimerA();
}

/* InitializePins
 * Parameters - None
 * Return - None
 * Algorithm - Calls appropriate initialization methods for the LED, Go button, Motor driver, limit switches, solenoid, and UART
 */
void InitializePins(void){
    InitializeLEDPortPin();
    InitializeGoButtonPortPin();
    InitializeMotorDriver();
    InitializeLimitSwitches();
    InitializeSolenoid();
    InitializeUART();
}

/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;
	//------------------------------- Configure the Clock -----------------------------------
    ConfigureClock();
	//------------------------------ Initialize Pins -------------------------------------------
	_enable_interrupts();
	InitializePins();
    moveToOrigin();
	while(1){
	    if(System_Active == 1){

	        while(System_Active == 1){
	            if(Word_Ready == 1){
	                Word_Ready = 0;
	                highlightWord(wordX1,wordY1,wordX2,wordY2);
	            }
	        }
	        moveToOrigin();
	        System_Active = 0;
            xDest = 0;
            yDest = 0;
	        SET_LED_LOW;
	        InitializePins();
	    }
	}return 0;
}
