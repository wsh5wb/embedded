#include "motorDriver.h"
#include "UART.h"
#include "TimerA.h"

extern unsigned int xPt; // Current x-location of the pen
extern unsigned int yPt; // Current y-location of the pen
extern unsigned int xDest; // x-destination of the pen
extern unsigned int yDest; // y-destination of the pen
int xDir = 1; // 1 for forward, -1 for back
int yDir = 1; // 1 for forward, -1 for back
int m1Step = 0; // 0 - low, 1 - high
int m2Step = 0; // 0 - low, 1 - high
int m1Active = 0; // 0 - inactive, 1 - active
int m2Active = 0; // 0 - inactive, 1 - active
int xCount = 0; // current number of x steps taken
int yCount = 0; // current number of y steps taken

/* InitializeMotorDriver
 * Parameters - None
 * Return - None
 * Algorithm - Calls two methods to initialize the port pins for all of the m1 and m2 outputs
 */
void InitializeMotorDriver(void){
    InitializeMotorDriverPinDirections();
    InitializeMotorDriverPinValues();
}

/* InitializeMotorDriverPinDirections
 * Parameters - None
 * Return - None
 * Algorithm - Sets M1 and M2 step, enable, MS1, reset, and dir to outputs
 */
void InitializeMotorDriverPinDirections(void){
    SET_M1_STEP_AS_AN_OUTPUT;
    SET_M2_STEP_AS_AN_OUTPUT;
    SET_M1_ENABLE_AS_OUTPUT;
    SET_M2_ENABLE_AS_OUTPUT;
    SET_M1_MS1_AS_OUTPUT;
    SET_M2_MS1_AS_OUTPUT;
    SET_M1_RESET_AS_AN_OUTPUT;
    SET_M2_RESET_AS_AN_OUTPUT;
    SET_M1_DIR_AS_OUTPUT;
    SET_M2_DIR_AS_OUTPUT;
}

/* InitializeMotorDriverPinValues
 * Parameters - None
 * Return - None
 * Algorithm - Sets M1 and M2 step, enable, MS1, reset, and dir to inactive state and sets up microstepping
 */
void InitializeMotorDriverPinValues(void){
    M1_MS1_HALF_STEP;
    M2_MS1_HALF_STEP;
    M1_ENABLE_HIGH;
    M2_ENABLE_HIGH;
    M1_DIR_FORWARD;
    M2_DIR_FORWARD;
    M1_RESET_SET_LOW;
    M2_RESET_SET_LOW;
    M1_STEP_SET_LOW;
    M2_STEP_SET_LOW;
}

/* moveToX
 * Parameters - (x, unsigned int, expected range: 0 - 500)
 * Return - None
 * Algorithm - Sets the motor direction to go to the correct x location (from the x parameter) and enables interrupts for PWM for M1.
 * It also updates the xDest, xDir, and m1Active variables.
 */
void moveToX(unsigned int x){
    int xDelta = x - xPt; // Negative indicates need to reverse the motor direction
    xDest = x;
    if(xDelta==0) {return;}
    if(xDelta < 0){
        setXMotorBackward();
        xDir = -1;
    }
    else{
        setXMotorForward();
        xDir = 1;
    }
    M1_ENABLE_LOW;
    m1Active = 1;
    enableM1Interrupts();
}

/* moveToY
 * Parameters - (y, unsigned int, expected range: 0 - 500)
 * Return - None
 * Algorithm - Sets the motor direction to go to the correct y location (from the y parameter) and enables interrupts for PWM for M2.
 * It also updates the yDest, yDir, and m2Active variables.
 */
void moveToY(unsigned int y){
    int yDelta = y - yPt; // Negative indicates need to reverse the motor direction
    yDest = y;
    if(yDelta==0) {return;}
    if(yDelta < 0){
        setYMotorBackward();
        yDir = -1;
    }
    else{
        setYMotorForward();
        yDir = 1;
    }
    M2_ENABLE_LOW;
    m2Active = 1;
    enableM2Interrupts();
}

/* moveTo
 * Parameters - (x, int, expected range: 0 - 500), (y, int, expected range: 0 - 500)
 * Return - None
 * Algorithm - Calls the moveToX and moveToY methods that handle the motors to move to those points. It doesn't return until
 * those methods finish
 */
void moveTo(unsigned int x, unsigned int y){
    moveToX(x);
    moveToY(y);
    while(m1Active || m2Active){}
}

/* highlightWord
 * Parameters - (x1, unsigned int, expected range: 0 - 125), (y1, unsigned int, expected range: 0 - 125),
 *              (x2, unsigned int, expected range: 0 - 125), (y2, unsigned int, expected range: 0 - 125)
 * Return - None
 * Algorithm - Note: x1, x2, y1, and y2 will all be shifted left by 2 to increase the internal precision and line smoothness
 *             Lifts up the pen and calls Bresenham to move to the starting location (x1,y1). It then puts down the pen and uses
 *             Bresenham to move to the final location (x2,y2) to highlight the word. At the end it acks to the Jetson.
 * those methods finish
 */
void highlightWord(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2){
    PowerSolenoid();
    bresenham(xPt,yPt,x1<<2,y1<<2);
    TurnOffSolenoid();
    bresenham(x1<<2, y1<<2, x2<<2, y2<<2);
    PowerSolenoid();
    UARTSendByte(0xFF);
}

/* setXMotorForward
 * Parameters - None
 * Return - None
 * Algorithm - Sets the m1 motor direction forward and then does a reset and full step so that it takes effect.
 */
void setXMotorForward(void){
    M1_DIR_FORWARD;
    M1_RESET_SET_HIGH;
    M1_STEP_SET_LOW;
    M1_STEP_SET_HIGH;
    M1_STEP_SET_LOW;
}

/* setYMotorForward
 * Parameters - None
 * Return - None
 * Algorithm - Sets the m2 motor direction forward and then does a reset and full step so that it takes effect.
 */
void setYMotorForward(void){
    M2_DIR_FORWARD;
    M2_RESET_SET_HIGH;
    M2_STEP_SET_LOW;
    M2_STEP_SET_HIGH;
    M2_STEP_SET_LOW;
}

/* setXMotorBackward
 * Parameters - None
 * Return - None
 * Algorithm - Sets the m1 motor direction backward and then does a reset and full step so that it takes effect.
 */
void setXMotorBackward(void){
    M1_DIR_BACKWARD;
    M1_RESET_SET_HIGH;
    M1_STEP_SET_LOW;
    M1_STEP_SET_HIGH;
    M1_STEP_SET_LOW;
}

/* setYMotorBackward
 * Parameters - None
 * Return - None
 * Algorithm - Sets the m2 motor direction backward and then does a reset and full step so that it takes effect.
 */
void setYMotorBackward(void){
    M2_DIR_BACKWARD;
    M2_RESET_SET_HIGH;
    M2_STEP_SET_LOW;
    M2_STEP_SET_HIGH;
    M2_STEP_SET_LOW;
}

/* bresenham
 * Parameters - (x1, unsigned int, expected range: 0 - 500), (y1, unsigned int, expected range: 0 - 500),
 *              (x2, unsigned int, expected range: 0 - 500), (y2, unsigned int, expected range: 0 - 500)
 * Return - None
 * Algorithm - Runs the bresenham line algorithm to calculate the next point to go to for drawing straight lines.
 *             Calls the moveTo method once it determines the correct location
 */
void bresenham(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2){
    int dx = x2 - x1, dy = y2 - y1;
    int sx = 1, sy = 1;
    int e2, err;
    if (dx < 0){
        dx *= -1;
        sx = -1;
    }if (dy < 0){
        dy *= -1;
        sy = -1;
    }err = dx - dy;
    for(;;){
        if (x1==x2 && y1==y2)     break;
        e2 = 2*err;
        if( e2 > -1 *dx){
            err -= dy;
            x1 += sx;
        }if (e2 < dy){
            err += dx;
            y1 += sy;
        }
        moveTo(x1,y1);
    }
}

/* A0ISR
 * Parameters - None
 * Return - None
 * Algorithm - ISR handler for Timer A0 PWM - used for M1. It toggles M1_Step and increments xCount and xPt as it moves. It handles stopping
 *             the motor when it reaches its destination and clearing the interrupt flag and updating the appropriate variables.
 */
#pragma vector = TIMER0_A0_VECTOR
__interrupt
void A0ISR(void){
    if(xPt == xDest || xPt == X_MAX){
        M1_STEP_SET_LOW;
        M1_ENABLE_HIGH;
        M1_RESET_SET_LOW;
        m1Step = 0;
        m1Active = 0;
        TA0CCTL0 &= ~CCIE;
        return;
    }
    M1_STEP_TOGGLE;
    TA0CCR0 &= ~CCIFG;
    if(m1Step == 0){ //transition from low to high is a step
        xCount++;
        m1Step = 1;
        if(xCount == STEPS_PER_POINT){
            xCount = 0;
            xPt += xDir;
        }
    }else{m1Step = 0;}
    TA0CCR0 &= ~CCIFG;
}

/* A1ISR
 * Parameters - None
 * Return - None
 * Algorithm - ISR handler for Timer A1 PWM - used for M2. It toggles M2_Step and increments yCount and yPt as it moves. It handles stopping
 *             the motor when it reaches its destination and clearing the interrupt flag and updating the appropriate variables.
 */
#pragma vector = TIMER1_A0_VECTOR
__interrupt
void A1ISR(void){
    if(yPt == yDest || yPt == Y_MAX){
        M2_STEP_SET_LOW;
        M2_ENABLE_HIGH;
        M2_RESET_SET_LOW;
        m2Step = 0;
        m2Active = 0;
        TA1CCTL0 &= ~CCIE;
        return;
    }
    M2_STEP_TOGGLE;
    if(m2Step == 0){ //transition from low to high is a step
        yCount++;
        m2Step = 1;
        if(yCount == STEPS_PER_POINT){
            yCount = 0;
            yPt += yDir;
        }
    }else{m2Step = 0;}
    TA1CCR0 &= ~CCIFG;
}
