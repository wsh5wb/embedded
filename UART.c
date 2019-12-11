#include "UART.h"

extern int Word_Ready;
extern int wordX1,wordX2,wordY1,wordY2;

/* InitializeUART
 * Parameters - None
 * Return - None
 * Algorithm - Handles setting UCA0 to UART mode and configuring the Rx and Tx pins to inputs for UART. It also sets UART
 *             to use SMCLK and a baud rate of 115200 and enables receive interrupts.
 */
void InitializeUART(){
    UCA0CTLW0 |= UCSWRST; //Set UCSWRST to one -- enables software reset
    UCA0CTLW0 &= ~UCSYNC; //Set UCSYNC to zero
    UCA0CTLW0 &= ~UCMODE; //Sets to UART mode

    //Sets the baud rate and clock
    UCA0CTLW0 |= UCSSEL_2; //Sets to SMCLK
    UCA0MCTLW |= UCOS16; //enable UCOS16
    UCA0BRW = 4;
    UCA0MCTLW |= 0xEE80;

    //Configure pins
    SET_RX_AS_INPUT;
    SET_TX_AS_OUTPUT;
    P1SEL0 |= TX_BIT + RX_BIT;

    UCA0CTLW0 &= ~UCSWRST; //Set UCSWRST to zero -- disables software reset
    UCA0IE |= UCRXIE; // Enable interrupts when receiving
    count = 0;
}

/* UARTReceiveByte
 * Parameters - None
 * Return - (unsigned int, expected range: 0-255)
 * Algorithm - Gets the value from the RX buffer, polls to make sure it's done receiving, and returns the received value.
 */
unsigned int UARTReceiveByte()
{
    unsigned int ReceiveValue = 0;

    ReceiveValue = UCA0RXBUF;

    while(UCA0STATW & UCBUSY);

    return ReceiveValue;
}

/* UARTSendByte
 * Parameters - (sendVal, unsigned int, expected range: 0-255)
 * Return - None
 * Algorithm - Sets the Tx buffer equal to the byte to be sent (sendVal input) and polls to make sure it's not still transmitting before
 *              returning.
 */
void UARTSendByte(unsigned int sendVal){

    while(!(UCA0IFG & UCTXIFG));

    UCA0TXBUF = sendVal;

    while(UCA0STATW & UCBUSY);

}

/* RxISR
 * Parameters - None
 * Return - None
 * Algorithm - ISR called when a byte is received. It checks this value to see if it's the reserved 255 meaning the word search is solved.
 *              If not, it goes through the four cases to get the x1, y1, x2, and y2 points used for highlighting the words. When this is
 *              finished, it acks back to the Jetson and sets word_ready to 1 to let it know the new points are in.
 */
#pragma vector = EUSCI_A0_VECTOR
__interrupt
void RxISR(void){
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
    }
    // Clear the UCA interrupt flag associated with RX
    UCA0IFG &= ~UCRXIFG;
}

