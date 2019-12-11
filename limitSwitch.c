#include "limitSwitch.h"

/* InitializeLimitSwitches
 * Parameters - None
 * Return - None
 * Algorithm - Sets the two limit switches to inputs
 */
void InitializeLimitSwitches(void){
    SET_LS1_AS_INPUT;
    SET_LS2_AS_INPUT;
}

/* readLS1
 * Parameters - None
 * Return - (int, expected range: 0-1)
 * Algorithm - Polls the port pin associated with LS1 - the limit switch used for the x motor and returns the current value
 *             0 - Switch open
 *             1 - Switch closed (Defines the axis)
 */
int readLS1(){
    return LS1_PORT & LS1_BIT;
}

/* readLS2
 * Parameters - None
 * Return - (int, expected range: 0-1)
 * Algorithm - Polls the port pin associated with LS2 - the limit switch used for the y motor and returns the current value
 *             0 - Switch open
 *             1 - Switch closed (Defines the axis)
 */
int readLS2(){
    return LS2_PORT & LS2_BIT;
}
