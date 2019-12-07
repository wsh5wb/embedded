#ifndef LIMITSWITCH_H
#define LIMITSWITCH_H

#include <msp430fr2476.h>

// x-axis switch
#define LS1_BIT BIT1
#define LS1_DDR P1DIR
#define LS1_PORT P1IN
#define SET_LS1_AS_INPUT LS1_DDR &= ~LS1_BIT

// y-axis switch
#define LS2_BIT BIT2
#define LS2_DDR P1DIR
#define LS2_PORT P1IN
#define SET_LS2_AS_INPUT LS2_DDR &= ~LS2_BIT

void InitializeLimitSwitches(void);
int readLS1();
int readLS2();

#endif
