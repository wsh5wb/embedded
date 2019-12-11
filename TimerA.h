#ifndef TIMER_A_H
#define TIMER_A_H

#include <msp430.h>

#define TA0CCR0_VALUE 53000   // Timer A period is TACCR0+1
#define TA1CCR0_VALUE 53000   // Timer A period is TACCR0+1

// Prototypes
void ConfigureTimerA(void);
void delay(unsigned int resolution);
void enableM1Interrupts();
void enableM2Interrupts();

#endif
