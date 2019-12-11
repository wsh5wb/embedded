#include "go.h"
#include "LED.h"
#include "UART.h"
#include "solenoid.h"

extern int System_Active;

/* InitializeGoButtonPortPin
 * Parameters - None
 * Return - None
 * Algorithm - Configures the go button port pin to an input and initializes the interrupt
 */
void InitializeGoButtonPortPin(void){
    // Configure port pin for pushbutton
    SET_GO_AS_INPUT;
    ENABLE_GO_INTERRUPT; //PAGE 311 - What do I need to do?
    SET_GO_INTERRUPT_HIGH_TO_LOW;
    P1IFG &= ~GO_BIT;
}

/* GoButtonISR
 * Parameters - None
 * Return - None
 * Algorithm - Runs when the go button is pressed. Sets the system active, turns the LED on, alerts the Jetson
 */
#pragma vector = PORT1_VECTOR
__interrupt
void GoButtonISR(void){
    if(System_Active == 0){
        System_Active = 1;
        SET_LED_HIGH;
        UARTSendByte(0xFF);
    }
    // Clear the port 1 interrupt flag associated with the pushbutton - bit-masking.
    P1IFG &= ~GO_BIT;
}
