#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "System/system.h"
#include "System/logger.h"
#include <stdint.h>

volatile char phasestate = 0;

// callbacks
void timerCall(void);
void zeroCrossingListenerPhaseA(char edge);
void zeroCrossingListenerPhaseB(char edge);
void zeroCrossingListenerPhaseC(char edge);

int main(void)
{
    initUART();
    logMsg("__Starting system.c tester__");

    initComp();
    registerVoltageZeroCrossingListenerPhaseA(&zeroCrossingListenerPhaseA);
    registerVoltageZeroCrossingListenerPhaseB(&zeroCrossingListenerPhaseB);
    registerVoltageZeroCrossingListenerPhaseC(&zeroCrossingListenerPhaseC);

    initPWM();

    logMsg("Init Timers...");
    initTimers();

    logMsg("Enable global interrupts...");
    sei();

    setPWMDutyCycle(60);
    changePhaseState(phasestate);


    // f_motor = 5Hz
    // i_motor = 7
    // T_60deg = 1/(2*i_motor*f_motor) = 14ms

    startAfterMs1(7, &timerCall);

    while(1)
    {
        logMsg(" ... ");
    }

    return 0;
}

void timerCall(void)
{
    phasestate = (phasestate+1)%6;

    changePhaseState(phasestate);

    //setEnableCompA(0);
    //setEnableCompB(0);
    //setEnableCompC(0);

    switch(phasestate)
    {
        case 0:
        //setEnableCompB(1);
        break;

        case 1:
        //setEnableCompA(1);
        break;

        case 2:
        //setEnableCompC(1);
        break;

        case 3:
        //setEnableCompB(1);
        break;

        case 4:
        //setEnableCompA(1);
        break;

        case 5:
        //setEnableCompC(1);
        break;
    }

    startAfterMs1(7, &timerCall);
}

void zeroCrossingListenerPhaseA(char edge)
{
    logVar("phA", phasestate, 10);
}

void zeroCrossingListenerPhaseB(char edge)
{
}

void zeroCrossingListenerPhaseC(char edge)
{
}
