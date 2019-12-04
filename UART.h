#ifndef UART_H
#define UART_H

#include <msp430fr2476.h>
#include "motorDriver.h"

#define RX_BIT BIT5
#define TX_BIT BIT4

#define RX_DDR P1DIR
//#define RX_PORT P1IN
#define SET_RX_AS_INPUT RX_DDR &= ~RX_BIT

#define TX_DDR P1DIR
//#define TX_PORT P1OUT
#define SET_TX_AS_OUTPUT TX_DDR |= TX_BIT

extern int System_Active;

int count;
//unsigned int x1; //2 bytes
//unsigned int y1;

void InitializeUART();
void UARTSendByte(unsigned int sendVal);
unsigned int UARTReceiveByte();

#endif
