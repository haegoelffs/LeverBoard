
#include "System/system.h"
#include "System/logger.h"
#include "synchronize.h"

#include <stdint.h>

#define TIMING ((uint8_t)0)
#define MAX_DELTATIME ((uint8_t)50)

#define NR_MEASURE_POINTS 100

 static uint8_t phasestate;
 static uint16_t time60deg;
 static uint16_t measuredTime;
 static uint16_t deltaTime;

 uint16_t measureValues[NR_MEASURE_POINTS];
 volatile uint8_t measureNr = 0;

// functions
void zeroCrossingListenerPhaseA(char edge);
void zeroCrossingListenerPhaseB(char edge);
void zeroCrossingListenerPhaseC(char edge);

void measure();

void timeMeassurementOverflow(void);

void holdFrequenz(void);
void switchPhaseAndManageComps(void);

void startSynchronize(uint8_t phasestate_init, uint16_t time60deg_init)
{
    //logMsg("Start synchronize...");

    phasestate = phasestate_init;
    time60deg = time60deg_init;

    registerVoltageZeroCrossingListenerPhaseA(&zeroCrossingListenerPhaseA);
    registerVoltageZeroCrossingListenerPhaseB(&zeroCrossingListenerPhaseB);
    registerVoltageZeroCrossingListenerPhaseC(&zeroCrossingListenerPhaseC);

    setPWMDutyCycle(40);

    holdFrequenz();
}

void holdFrequenz(void)
{
    switchPhaseAndManageComps();

    startAfterUs(time60deg, &holdFrequenz);
}

void switchPhaseAndManageComps(void)
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
}

void zeroCrossingListenerPhaseA(char edge)
{
    setEnableCompA(0); // disable isr -> interrupt only on first zero crossing

    if(!isTimeMeasurementRunning())
    {
        startTimeMeasurement(&timeMeassurementOverflow);
    }
    else
    {
        measuredTime = (uint16_t)stopTimeMeasurement();
        deltaTime = measuredTime - time60deg;
        //measure();

        if(deltaTime > MAX_DELTATIME)
        {
            deltaTime = MAX_DELTATIME;
        }

        time60deg = measuredTime -deltaTime;
    }

    uint16_t temp = time60deg*((uint8_t)30-TIMING)/(uint8_t)60;
    /*
    time60deg_max = 2100
    TIMING_min = 0
    --> 2100*30 = 63000 = 0b1111'0110'0001'1000 --> 16bit ok
    */

    //startAfterUs(temp, &holdFrequenz);
}

void zeroCrossingListenerPhaseB(char edge)
{
    setEnableCompB(0); // disable isr -> interrupt only on first zero crossing

    if(!isTimeMeasurementRunning())
    {
        startTimeMeasurement(&timeMeassurementOverflow);
    }
    else
    {
        measuredTime = (uint16_t)stopTimeMeasurement();
        deltaTime = measuredTime - time60deg;
        //measure();

        if(deltaTime > MAX_DELTATIME)
        {
            deltaTime = MAX_DELTATIME;
        }

        time60deg = measuredTime -deltaTime;
    }

    uint16_t temp = time60deg*((uint8_t)30-TIMING)/(uint8_t)60;

    //startAfterUs(temp, &holdFrequenz);
}

void zeroCrossingListenerPhaseC(char edge)
{
    setEnableCompC(0); // disable isr -> interrupt only on first zero crossing

    if(!isTimeMeasurementRunning())
    {
        startTimeMeasurement(&timeMeassurementOverflow);
    }
    else
    {
        measuredTime = (uint16_t)stopTimeMeasurement();
        deltaTime = measuredTime - time60deg;
        //measure();

        if(deltaTime > MAX_DELTATIME)
        {
            deltaTime = MAX_DELTATIME;
        }

        time60deg = measuredTime -deltaTime;
    }

    uint16_t temp = time60deg*((uint8_t)30-TIMING)/(uint8_t)60;

    //startAfterUs(temp, &holdFrequenz);
}

void timeMeassurementOverflow(void)
{

}

void measure()
{
    if(measureNr < NR_MEASURE_POINTS)
    {
        measureValues[measureNr] = deltaTime;
        measureNr++;
    }
    else
    {
        logMsg("measured delta times:");
        measureNr = 0;
        while(measureNr <= NR_MEASURE_POINTS)
        {
            logUnsignedInt(measureValues[measureNr], 5);
            measureNr++;
        }
        writeNewLine();
        writeNewLine();
        writeNewLine();
        logNamedUnsignedInt("t60deg", time60deg, 20);

        while(1)
        {
        }
    }
}
