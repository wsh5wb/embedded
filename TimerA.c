#include "timerA.h"

/* ConfigureTimerA
 * Parameters - None
 * Return - None
 * Algorithm - Sets up Timer A for doing PWM on TA0CCR0 for M1 and TA1CCR0 for M2. It sets it in up mode and uses Smclk for its period.
 */
void ConfigureTimerA(void)
{
    TA0CTL = (MC_0 | TACLR | ID_3);// Stop and clear the timer.
    TA0CCR0 = TA0CCR0_VALUE;

    TA1CTL = (MC_0 | TACLR | ID_3);// Stop and clear the timer.
    TA1CCR0 = TA1CCR0_VALUE;

    TA0CTL = (MC_1 | TASSEL_2); //Set in up mode and selects Smclk
    TA1CTL = (MC_1 | TASSEL_2);
}

/* delay
 * Parameters - (resolution, unsigned int, expected: 0 - max of unsigned int)
 * Return - None
 * Algorithm - Runs a for loop resolution time just to add some delay.
 */
void delay(unsigned int resolution){
    int i;
    for(i=0; i<resolution;i++){}

}

/* enableM1Interrupts
 * Parameters - None
 * Return - None
 * Algorithm - Enables interrupts for M1
 */
void enableM1Interrupts(){
    TA0CCTL0 |= CCIE;
}

/* enableM2Interrupts
 * Parameters - None
 * Return - None
 * Algorithm - Enables interrupts for M2
 */
void enableM2Interrupts(){
    TA1CCTL0 |= CCIE;
}


