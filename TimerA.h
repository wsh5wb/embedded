#ifndef TIMER_A_H
#define TIMER_A_H

#include <msp430.h>

#define TA0CCR0_VALUE 50000//65500   // Timer A period is TACCR0+1
#define TA1CCR0_VALUE 55000//65500   // Timer A period is TACCR0+1

// Prototypes
void ConfigureTimerA(void);
void delay(unsigned int resolution);
void enableM1Interrupts();
//void disableM1Interrupts();
void enableM2Interrupts();
//void disableM2Interrupts();

#endif
