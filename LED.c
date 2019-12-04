#include "LED.h"

void InitializeLEDPortPin(void)
{
    // Initialize the value and port pin direction of the LEDs.
    SET_LED_AS_OUTPUT;
    SET_LED_LOW;
}

void TurnLEDOn(void){
    SET_LED_HIGH;
}

void TurnLEDOff(void){
    SET_LED_LOW;
}
