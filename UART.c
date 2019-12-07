#include "UART.h"

extern int Word_Ready;
extern int wordX1,wordX2,wordY1,wordY2;

void InitializeUART(){
    UCA0CTLW0 |= UCSWRST; //Set UCSWRST to one -- enables software reset
    UCA0CTLW0 &= ~UCSYNC; //Set UCSYNC to zero
    UCA0CTLW0 &= ~UCMODE; //Sets to UART mode
    //UCA0CTLW0 |= UCMSB;   //Sets MSB first
    //NOTE: If want automatic baud-rate detection, change above line to UCA0CTLW0 |= UCMODE;

    UCA0CTLW0 |= UCSSEL_2; //Sets to SMCLK
    UCA0MCTLW |= UCOS16; //enable UCOS16
    UCA0BRW = 4;
    UCA0MCTLW |= 0xEE80;

    SET_RX_AS_INPUT;
    SET_TX_AS_OUTPUT;

    P1SEL0 |= TX_BIT + RX_BIT; //Configure pins
    UCA0CTLW0 &= ~UCSWRST; //Set UCSWRST to zero -- disables software reset

    //TO DO: CHeck if should only be transmitting ------------------------------------------------------------------
    UCA0IE |= UCRXIE; // Enable interrupts when receiving
    //UCA0IE |= UCRXIE + UCTXIE; // Enable interrupts when receiving and transmitting
    count = 0;
}

unsigned int UARTReceiveByte()
{
    unsigned int ReceiveValue = 0;

    ReceiveValue = UCA0RXBUF;

    while(UCA0STATW & UCBUSY);

    return ReceiveValue;
}

void UARTSendByte(unsigned int sendVal){

    while(!(UCA0IFG & UCTXIFG));

    UCA0TXBUF = sendVal;

    while(UCA0STATW & UCBUSY);

}

__attribute__((interrupt(EUSCI_A0_VECTOR)))
void RxISR(void){
    //if(UCIV__UCRXIFG) - Might not need if only enable RX interrupts
    unsigned int receivedByte = UARTReceiveByte();
    if(receivedByte == 0xFF){
        System_Active = 0;
        UCA0IFG &= ~UCRXIFG;
        return;
    }

    switch(count){
    case 0:
        wordX1 = receivedByte;
        count++;
        break;
    case 1:
        wordY1 = receivedByte;
        count++;
        break;
    case 2:
        wordX2 = receivedByte;
        count++;
        break;
    case 3:
        wordY2 = receivedByte;
        count = 0;
        Word_Ready = 1;
        UARTSendByte(0xFF);
        break;

    //Send ACK?

    }

    // Clear the UCA interrupt flag associated with RX
    UCA0IFG &= ~UCRXIFG;
}
/*
__attribute__((interrupt(EUSCI_A0_VECTOR)))
void TxISR(void){


    // Clear the UCA interrupt flag associated with TX
    UCA0IFG &= ~UCTXIFG;
}*/
