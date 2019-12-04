#ifndef LED_H
#define LED_H

#include <msp430fr2476.h>

#define LED_BIT BIT3
#define LED_PORT P1OUT //Check when it comes time if it's active high or low
#define LED_DDR P1DIR
#define SET_LED_AS_OUTPUT LED_DDR |= LED_BIT
#define SET_LED_HIGH LED_PORT |= LED_BIT
#define SET_LED_LOW LED_PORT &= ~LED_BIT
#define TOGGLE_LED LED_PORT ^= LED_BIT

// Prototypes
void InitializeLEDPortPin(void);
void TurnLEDOn(void);
void TurnLEDOff(void);

#endif
