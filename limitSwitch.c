#include "limitSwitch.h"

void InitializeLimitSwitches(void){
    SET_LS1_AS_INPUT;
    SET_LS2_AS_INPUT;
}

int readLS1(){
    return LS1_PORT & LS1_BIT;
}

int readLS2(){
    return LS2_PORT & LS2_BIT;
}
