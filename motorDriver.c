#include "motorDriver.h"
#include "UART.h"

unsigned int xPt = 0;
unsigned int yPt = 0;
unsigned int xDest = 0;
unsigned int yDest = 0;
int xDir = 1; // 1 for forward, -1 for back
int yDir = 1;
int m1Step = 0; // 0 - low, 1 - high
int m2Step = 0;
int m1Active = 0;
int m2Active = 0;
int xCount = 0;
int yCount = 0;

void InitializeMotorDriver(void){
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

    //Have to initialize these values
    M1_MS1_HALF_STEP;
    M2_MS1_HALF_STEP;
    M1_ENABLE_HIGH; //check if low or high
    M2_ENABLE_HIGH;
    //set direction forward
    M1_DIR_FORWARD;
    M2_DIR_FORWARD;

    M1_RESET_SET_LOW;
    M2_RESET_SET_LOW;
    M1_STEP_SET_LOW;
    M2_STEP_SET_LOW;
}

void moveToX(int x){
    xDest = x;
    int xDelta = x - xPt; // Negative indicates need to reverse the motor direction
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

void moveToY(int y){
    yDest = y;
    int yDelta = y - yPt; // Negative indicates need to reverse the motor direction
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

void moveTo(unsigned int x, unsigned int y){
    moveToX(x);
    moveToY(y);
    while(m1Active || m2Active){}
}

void highlightWord(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2){
    moveTo(x1,y1);
    TurnOffSolenoid();
    bresenham(x1, y1, x2, y2);//moveTo(x2,y2);
    PowerSolenoid();
    UARTSendByte(0xFF);
}

void setXMotorForward(void){
    M1_DIR_FORWARD;
    M1_RESET_SET_HIGH;
    M1_STEP_SET_LOW;
    M1_STEP_SET_HIGH;
    M1_STEP_SET_LOW;
}

void setYMotorForward(void){
    M2_DIR_FORWARD;
    M2_RESET_SET_HIGH;
    M2_STEP_SET_LOW;
    M2_STEP_SET_HIGH;
    M2_STEP_SET_LOW;
}

void setXMotorBackward(void){
    M1_DIR_BACKWARD;
    M1_RESET_SET_HIGH;
    M1_STEP_SET_LOW;
    M1_STEP_SET_HIGH;
    M1_STEP_SET_LOW;
}

void setYMotorBackward(void){
    M2_DIR_BACKWARD;
    M2_RESET_SET_HIGH;
    M2_STEP_SET_LOW;
    M2_STEP_SET_HIGH;
    M2_STEP_SET_LOW;
}

void bresenham(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2){
    int dx = x2 - x1, dy = y2 - y1;
    int sx = 1, sy = 1;
    int err = dx - dy;
    int e2;

    if (dx < 0){
        dx *= 1;
        sx = -1;
    }

    if (dy < 0){
        dy *= 1;
        sx = -1;
    }

    for(;;){
        if (x1==x2 && y1 == y2)     break;

        e2 = 2*err;

        if( e2 > -1 *dx){
            err -= dy;
            x1 += sx;
        }

        if (e2 < dy){
            err += dx;
            y1 += sy;
        }
        moveTo(x1, y1);
    }

}

__attribute__((interrupt(TIMER0_A0_VECTOR)))
void A0ISR(void){
    if(xPt == xDest){
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
        if(xCount == 20){
            xCount = 0;
            xPt += xDir;
        }
    }else{m1Step = 0;}

    TA0CCR0 &= ~CCIFG;

}

__attribute__((interrupt(TIMER1_A0_VECTOR)))
void A1ISR(void){
    if(yPt == yDest){
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
        if(yCount == 20){
            yCount = 0;
            yPt += yDir;
        }
    }else{m2Step = 0;}

    TA1CCR0 &= ~CCIFG;

}
