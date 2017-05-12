
#include "System/system.h"
#include "System/logger.h"
#include "synchronize.h"

#include <stdint.h>

#define MEASURE_POSITION_A_ZEROCROSSING
//#define MEASURE_POSITION_B_ZEROCROSSING
//#define MEASURE_TIME_60DEG

#define TIMING ((uint8_t)0)
#define MAX_DELTATIME ((uint8_t)50)

#define NR_MEASURE_POINTS 200

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

    setPWMDutyCycle(25);

    holdFrequenz();
}

void holdFrequenz(void)
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

    #ifdef MEASURE_POSITION_A_ZEROCROSSING
    if(!isTimeMeasurementRunning())
    {
        startTimeMeasurement(&timeMeassurementOverflow);
    }
    #endif // MEASURE_POSITION_A_ZEROCROSSING

    #ifdef MEASURE_POSITION_B_ZEROCROSSING
    if(isTimeMeasurementRunning())
    {
        measuredTime = (uint16_t)stopTimeMeasurement();
        measure();
    }
    #endif // MEASURE_POSITION_B_ZEROCROSSING

    startAfterUs(time60deg, &holdFrequenz);
}

void zeroCrossingListenerPhaseA(char edge)
{
    setEnableCompA(0); // disable isr -> interrupt only on first zero crossing

    #ifdef MEASURE_POSITION_A_ZEROCROSSING
    if(isTimeMeasurementRunning())
    {
        measuredTime = (uint16_t)stopTimeMeasurement();
        measure();
    }
    #endif // MEASURE_POSITION_A_ZEROCROSSING

    #ifdef MEASURE_POSITION_B_ZEROCROSSING
    if(!isTimeMeasurementRunning())
    {
        startTimeMeasurement(&timeMeassurementOverflow);
    }
    #endif // MEASURE_POSITION_B_ZEROCROSSING

    #ifdef MEASURE_TIME_60DEG
    if(!isTimeMeasurementRunning())
    {
        startTimeMeasurement(&timeMeassurementOverflow);
    }
    else
    {
        measuredTime = (uint16_t)stopTimeMeasurement();
        measure();
    }
    #endif // MEASURE_TIME_60DEG

    //uint16_t temp = time60deg*((uint8_t)30-TIMING)/(uint8_t)60;
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

    #ifdef MEASURE_POSITION_A_ZEROCROSSING
    if(isTimeMeasurementRunning())
    {
        measuredTime = (uint16_t)stopTimeMeasurement();
        measure();
    }
    #endif // MEASURE_POSITION_A_ZEROCROSSING

    #ifdef MEASURE_POSITION_B_ZEROCROSSING
    if(!isTimeMeasurementRunning())
    {
        startTimeMeasurement(&timeMeassurementOverflow);
    }
    #endif // MEASURE_POSITION_B_ZEROCROSSING

    #ifdef MEASURE_TIME_60DEG
    if(!isTimeMeasurementRunning())
    {
        startTimeMeasurement(&timeMeassurementOverflow);
    }
    else
    {
        measuredTime = (uint16_t)stopTimeMeasurement();
        measure();
    }
    #endif // MEASURE_TIME_60DEG
}

void zeroCrossingListenerPhaseC(char edge)
{
    setEnableCompC(0); // disable isr -> interrupt only on first zero crossing

   #ifdef MEASURE_POSITION_A_ZEROCROSSING
    if(isTimeMeasurementRunning())
    {
        measuredTime = (uint16_t)stopTimeMeasurement();
        measure();
    }
    #endif // MEASURE_POSITION_A_ZEROCROSSING

    #ifdef MEASURE_POSITION_B_ZEROCROSSING
    if(!isTimeMeasurementRunning())
    {
        startTimeMeasurement(&timeMeassurementOverflow);
    }
    #endif // MEASURE_POSITION_B_ZEROCROSSING

    #ifdef MEASURE_TIME_60DEG
    if(!isTimeMeasurementRunning())
    {
        startTimeMeasurement(&timeMeassurementOverflow);
    }
    else
    {
        measuredTime = (uint16_t)stopTimeMeasurement();
        measure();
    }
    #endif // MEASURE_TIME_60DEG
}

void timeMeassurementOverflow(void)
{
    logMsg("ERROR");
}

void measure()
{
    if(measureNr < NR_MEASURE_POINTS)
    {
        measureValues[measureNr] = measuredTime;
        measureNr++;
    }
    else
    {
        logMsg("measured values:");
        measureNr = 0;
        while(measureNr <= NR_MEASURE_POINTS)
        {
            logUnsignedInt(measureValues[measureNr], 5);
            measureNr++;
        }

        logNamedUnsignedInt("t60deg", time60deg, 20);

        while(1)
        {
        }
    }
}
