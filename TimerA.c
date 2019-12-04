#include "timerA.h"

void ConfigureTimerA(void)
{
    TA0CTL = (MC_0 | TACLR | ID_3);// Stop and clear the timer.
    TA0CCR0 = TA0CCR0_VALUE;
    //TA0CCTL0 |= CCIE; // enable interrupts
    TA0CCTL0 |= OUTMOD_7; //reset-set

    TA1CTL = (MC_0 | TACLR | ID_3);// Stop and clear the timer.
    TA1CCR0 = TA1CCR0_VALUE;
    //TA1CCTL0 |= CCIE; // enable interrupts
    TA1CCTL0 |= OUTMOD_7; //reset-set

    //Connect to port pins

    // TO DO: Select clock source
    TA0CTL = (MC_1 | TASSEL_2); //Set in up mode and selects Smclk
    TA1CTL = (MC_1 | TASSEL_2);
}

void delay(unsigned int resolution){
    int i;
    for(i=0; i<resolution;i++){}

}

void enableM1Interrupts(){
    TA0CCTL0 |= CCIE;
}

void enableM2Interrupts(){
    TA1CCTL0 |= CCIE;
}


