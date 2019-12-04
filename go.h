#ifndef GO_H
#define GO_H

#include <msp430fr2476.h>

#define GO_BIT BIT0
#define GO_DDR P1DIR
#define GO_PORT P1OUT
#define GO_REN P1REN
#define SET_GO_AS_INPUT GO_DDR &= ~GO_BIT
#define ENABLE_GO_PULLUP GO_REN |= GO_BIT
#define SET_GO_ACTIVE_HIGH GO_PORT |= GO_BIT
#define SET_GO_ACTIVE_LOW GO_PORT &= ~GO_BIT
#define ENABLE_GO_INTERRUPT P1IE |= GO_BIT
#define SET_GO_INTERRUPT_HIGH_TO_LOW P1IES |= GO_BIT

// Prototypes
void InitializeGoButtonPortPin(void);


#endif
