#include "solenoid.h"

/* InitializeSolenoid
 * Parameters - None
 * Return - None
 * Algorithm - Configures the solenoid port pin to an output and sets it low
 */
void InitializeSolenoid(void){
    SET_SOL_AS_OUTPUT;
    SET_SOL_LOW;
}

/* PowerSolenoid
 * Parameters - None
 * Return - None
 * Algorithm - Sets the solenoid pin to a high output
 */
void PowerSolenoid(void){
    SET_SOL_HIGH;
}

/* TurnoffSolenoid
 * Parameters - None
 * Return - None
 * Algorithm - Sets the solenoid pin to a low output
 */
void TurnOffSolenoid(void){
    SET_SOL_LOW;
}
