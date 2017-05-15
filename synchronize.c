
#include "System/system.h"
#include "System/logger.h"
#include "System/loggerISR.h"
#include "synchronize.h"

#include <stdint.h>

#define TIMING ((uint8_t)1)

#define P_DIVIDER 200
#define I_DIVIDER 256

 static uint8_t phasestate;
 static uint16_t time60deg;

// functions
void zeroCrossingListenerPhaseA(char edge);
void zeroCrossingListenerPhaseB(char edge);
void zeroCrossingListenerPhaseC(char edge);

void timeMeassurementOverflow(void);

void switchPhases(void);

void startSynchronize(uint8_t phasestate_init, uint16_t time60deg_init)
{
    phasestate = phasestate_init;
    time60deg = time60deg_init;

    registerVoltageZeroCrossingListenerPhaseA(&zeroCrossingListenerPhaseA);
    registerVoltageZeroCrossingListenerPhaseB(&zeroCrossingListenerPhaseB);
    registerVoltageZeroCrossingListenerPhaseC(&zeroCrossingListenerPhaseC);

    setPWMDutyCycle(25);

    setEnableCompA(1);
    setEnableCompB(1);
    setEnableCompC(1);

    switchPhases();
}

void switchPhases(void)
{
    phasestate = (phasestate+1)%6;
    changePhaseState(phasestate);

    if(isTimeMeasurementRunning())
    {
        stopTimeMeasurement();
        logMsg("Fatal Error: Time not stopped");
    }

    startTimeMeasurement(&timeMeassurementOverflow);
    startAfterUs(time60deg, &switchPhases);
}

void zeroCrossingCalculations()
{
    if(isTimeMeasurementRunning())
    {
        static uint16_t measuredTime;
        measuredTime = (uint16_t)stopTimeMeasurement();
    }
}

void zeroCrossingListenerPhaseA(char edge)
{
    if(phaseState == 1 || phaseState == 4)
    {
        zeroCrossingCalculations();
    }
}

void zeroCrossingListenerPhaseB(char edge)
{
    if(phaseState == 0 || phaseState == 3)
    {
        zeroCrossingCalculations();
    }
}

void zeroCrossingListenerPhaseC(char edge)
{
    if(phaseState == 2 || phaseState == 5)
    {
        zeroCrossingCalculations();
    }
}

void timeMeassurementOverflow(void)
{
    logMsg("Fatal Error: Timer overflow");
}
