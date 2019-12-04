#include "solenoid.h"

void InitializeSolenoid(void){
    SET_SOL_AS_OUTPUT;
    SET_SOL_LOW;
}

void PowerSolenoid(void){
    SET_SOL_HIGH;
}

void TurnOffSolenoid(void){
    SET_SOL_LOW;
}
