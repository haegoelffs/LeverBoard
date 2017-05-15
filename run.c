
#include "System/system.h"
#include "System/logger.h"
#include "System/loggerISR.h"
#include "synchronize.h"

#include <stdint.h>

#define TIMING ((uint8_t)1)
#define MAX_TIME_DIFFERENCE_LEADING ((uint8_t)0)
#define MAX_TIME_DIFFERENCE_LAGING ((uint8_t)0)

#define MAX_FAULT_POS 40
#define MAX_FAULT_NEG -40
//#define FAULT_DIVIDER (uint8_t)40
//#define FAULT_INTEGRAL_DIVIDER (uint8_t)256

#define P_DIVIDER 200
#define I_DIVIDER 256


#define NR_MEASURE_POINTS 200
//#define MEASURE

 static uint8_t phasestate;
 static uint16_t time60deg;
 static uint16_t measuredTime;
 static uint16_t targetTime;
 static int16_t fault;

 #ifdef MEASURE
 uint16_t measuredTimes[NR_MEASURE_POINTS];
 uint16_t measuredTargetValues[NR_MEASURE_POINTS];
 uint16_t calculatedFaults[NR_MEASURE_POINTS];
 uint16_t actualTime60deg[NR_MEASURE_POINTS];
 volatile uint8_t measureNr = 0;
 #endif // MEASURE

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
    phasestate = phasestate_init;
    time60deg = time60deg_init;

    registerVoltageZeroCrossingListenerPhaseA(&zeroCrossingListenerPhaseA);
    registerVoltageZeroCrossingListenerPhaseB(&zeroCrossingListenerPhaseB);
    registerVoltageZeroCrossingListenerPhaseC(&zeroCrossingListenerPhaseC);

    setPWMDutyCycle(35);

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

    if(isTimeMeasurementRunning())
    {
        stopTimeMeasurement();
        //logMsg("Time not stopped");
    }
    startTimeMeasurement(&timeMeassurementOverflow);

    startAfterUs(time60deg, &holdFrequenz);
}

int16_t controlCalc(int32_t target, int32_t actual)
{
    fault = (target - actual);

    static int32_t fault_I;
    fault_I = fault_I + fault;

    return (fault/P_DIVIDER + fault_I/I_DIVIDER);
}

void zeroCrossingCalculations()
{
    if(isTimeMeasurementRunning())
    {
        measuredTime = (uint16_t)stopTimeMeasurement();

        if(measuredTime > time60deg/2)
        {
            // ausreiser
            measuredTime = time60deg/2;
        }
    }

    /* data type calculations:
    time60deg_max = 2100
    TIMING_min = 0
    --> 2100*30 = 63000 = 0b1111'0110'0001'1000 --> 16bit ok
    */
    targetTime = (time60deg*(30-TIMING))/60;
    //time60deg = time60deg + controlCalc(targetTime, measuredTime);
    measure();
}

void zeroCrossingListenerPhaseA(char edge)
{
    setEnableCompA(0); // disable isr -> interrupt only on first zero crossing
    logMsg("crossing A");
    zeroCrossingCalculations();
}

void zeroCrossingListenerPhaseB(char edge)
{
    setEnableCompB(0); // disable isr -> interrupt only on first zero crossing
    logMsg("crossing B");
    zeroCrossingCalculations();
}

void zeroCrossingListenerPhaseC(char edge)
{
    setEnableCompC(0); // disable isr -> interrupt only on first zero crossing
    logMsg("crossing C");
    zeroCrossingCalculations();
}

void timeMeassurementOverflow(void)
{
    logMsg("Timer overflow");
}

void measure()
{
     #ifdef MEASURE
    if(measureNr < NR_MEASURE_POINTS)
    {
        measuredTimes[measureNr] = measuredTime;
        measuredTargetValues[measureNr] = targetTime;
        calculatedFaults[measureNr] = fault;
        actualTime60deg[measureNr] = time60deg;
        measureNr++;
    }
    else
    {
        logMsg("measured times:");
        measureNr = 0;
        while(measureNr <= NR_MEASURE_POINTS)
        {
            logUnsignedInt(measuredTimes[measureNr], 5);
            measureNr++;
        }

        logMsg("measured target values:");
        measureNr = 0;
        while(measureNr <= NR_MEASURE_POINTS)
        {
            logUnsignedInt(measuredTargetValues[measureNr], 5);
            measureNr++;
        }

        logMsg("calculated faults:");
        measureNr = 0;
        while(measureNr <= NR_MEASURE_POINTS)
        {
            logSignedInt(calculatedFaults[measureNr], 5);
            measureNr++;
        }

        logMsg("time 60 deg:");
        measureNr = 0;
        while(measureNr <= NR_MEASURE_POINTS)
        {
            logUnsignedInt(actualTime60deg[measureNr], 5);
            measureNr++;
        }

        while(1)
        {
        }
    }
    #endif // MEASURE
}
