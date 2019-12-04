#ifndef SOLENOID_H
#define SOLENOID_H

#include <msp430fr2476.h>

#define SOL_BIT BIT6
#define SOL_DDR P1DIR
#define SOL_PORT P1OUT
#define SET_SOL_AS_OUTPUT SOL_DDR |= SOL_BIT
#define SET_SOL_HIGH SOL_PORT |= SOL_BIT
#define SET_SOL_LOW SOL_PORT &= ~SOL_BIT
#define TOGGLE_SOL SOL_PORT ^= SOL_BIT

// Prototypes
void InitializeSolenoid(void);
void PowerSolenoid(void);
void TurnOffSolenoid(void);

#endif
