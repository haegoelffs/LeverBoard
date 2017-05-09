#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "System/system.h"
#include "System/logger.h"
#include <stdint.h>

volatile char phasestate = 0;
uint32_t time = 0;

// callbacks
void timerCall(void);

void zeroCrossingListenerPhaseA(char edge);
void zeroCrossingListenerPhaseB(char edge);
void zeroCrossingListenerPhaseC(char edge);

void timeMeassurementOverflow(void);

int main(void)
{
    initUART();
    writeNewLine();
    writeNewLine();
    logMsg("__Starting system.c tester__");

    initTimers();

    initComp();
    registerVoltageZeroCrossingListenerPhaseA(&zeroCrossingListenerPhaseA);
    registerVoltageZeroCrossingListenerPhaseB(&zeroCrossingListenerPhaseB);
    registerVoltageZeroCrossingListenerPhaseC(&zeroCrossingListenerPhaseC);

    initPWM();

    logMsg("Enable global interrupts...");
    sei();

    setPWMDutyCycle(50);
    changePhaseState(phasestate);


    // f_motor = 5Hz
    // i_motor = 7
    // T_60deg = 1/(i_motor*f_motor) = 28571us*/

    startAfterUs(28571, &timerCall);
    //startTimeMeasurement1(0);
    //startAfterUs(2000, &timerCall2);

     //logUnsignedInt("dt", (uint32_t)0xFFFFFFFF, 50);

    while(1)
    {
        //logMsg(" ... ");
    }

    return 0;
}

void timerCall(void)
{
    phasestate = (phasestate+1)%6;

    changePhaseState(phasestate);

    setEnableCompA(0);
    setEnableCompB(0);
    setEnableCompC(0);

    switch(phasestate)
    {
        case 0:
        setEnableCompB(1);
        break;

        case 1:
        setEnableCompA(1);
        break;

        case 2:
        setEnableCompC(1);
        break;

        case 3:
        setEnableCompB(1);
        break;

        case 4:
        setEnableCompA(1);
        break;

        case 5:
        setEnableCompC(1);
        break;
    }

    startAfterUs(14000, &timerCall);
}

void zeroCrossingListenerPhaseA(char edge)
{
    //startTimeMeasurement1(&timeMeassurementOverflow);

    if(edge)
    {
        logMsg("rising");
    }
    else
    {
        logMsg("falling");
    }
}

void zeroCrossingListenerPhaseB(char edge)
{

}

void zeroCrossingListenerPhaseC(char edge)
{

}

void timeMeassurementOverflow(void)
{

}
