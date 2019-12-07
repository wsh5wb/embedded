#include "go.h"
#include "LED.h"
#include "UART.h"
#include "solenoid.h"

extern int System_Active;
int butcount = 0;

// Prototypes
void InitializeGoButtonPortPin(void){
    // Configure port pin for pushbutton
    SET_GO_AS_INPUT;
    ENABLE_GO_INTERRUPT; //PAGE 311 - What do I need to do?
    SET_GO_INTERRUPT_HIGH_TO_LOW;
    P1IFG &= ~GO_BIT;
}

__attribute__((interrupt(PORT1_VECTOR)))
void GoButtonISR(void){
    butcount++;
    //TOGGLE_SOL;
    if(!System_Active){
        System_Active = 1;
        SET_LED_HIGH;
        UARTSendByte(0xFF);
    }
    // Clear the port 1 interrupt flag associated with the pushbutton - bit-masking.
    P1IFG &= ~GO_BIT;
}
