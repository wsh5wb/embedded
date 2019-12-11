#include "LED.h"

/* InitializeLEDPortPin
 * Parameters - None
 * Return - None
 * Algorithm - Configures the LED port pin to an output and sets it low
 */
void InitializeLEDPortPin(void)
{
    // Initialize the value and port pin direction of the LEDs.
    SET_LED_AS_OUTPUT;
    SET_LED_LOW;
}

/* TurnLEDOn
 * Parameters - None
 * Return - None
 * Algorithm - Sets the LED's pin to high so it turns on
 */
void TurnLEDOn(void){
    SET_LED_HIGH;
}

/* TurnLEDOff
 * Parameters - None
 * Return - None
 * Algorithm - Sets the LED's pin to low so it turns off
 */
void TurnLEDOff(void){
    SET_LED_LOW;
}
