
#include "System/system.h"
#include "System/logger.h"
#include "controlled.h"
#include "System/ringbufferDriveData.h"

#include <stdint.h>
#define MEASURE

#define TIMING 20
//#define P_DIVIDER 200
//#define P_DIVIDER 128
#define P_DIVIDER 64
#define I_DIVIDER 256
//#define MAX_T_60_DEG 2500
#define MAX_T_60_DEG 3000

static uint8_t phasestate;
static uint16_t time60deg;
static uint8_t isActive;

static void (*callback)(void);

// functions
void zeroCrossingListenerPhaseA(char edge);
void zeroCrossingListenerPhaseB(char edge);
void zeroCrossingListenerPhaseC(char edge);

void timeMeassurementOverflow(void);

void switchPhases(void);
void switchPhaseAndManageComps(void);

void startControlled(uint8_t phasestate_init, uint16_t time60deg_init, void (*tooSlowCallback)(void))
{
    phasestate = phasestate_init;
    time60deg = time60deg_init;
    callback = tooSlowCallback;

    registerVoltageZeroCrossingListenerPhaseA(&zeroCrossingListenerPhaseA);
    registerVoltageZeroCrossingListenerPhaseB(&zeroCrossingListenerPhaseB);
    registerVoltageZeroCrossingListenerPhaseC(&zeroCrossingListenerPhaseC);

    isActive = 1;
    switchPhases();
}

void stopControlled()
{
    isActive = 0;
}

void switchPhases(void)
{
    // start current measure before switching phase (max. current in phase)
    if(getPhaseState() == 3)
    {
        startMeasureProcedure('A');
    }

    phasestate = (phasestate+1)%6;
    changePhaseState(phasestate);

    // handle comperators
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
    }
    startTimeMeasurement(&timeMeassurementOverflow);

    if(isActive)
    {
        startAfterUs(time60deg, &switchPhases);
    }
}

void zeroCrossingCalculations()
{
    if(isTimeMeasurementRunning())
    {
        uint16_t measuredTime = (uint16_t)stopTimeMeasurement();

        setEnableCompA(0);
        setEnableCompB(0);
        setEnableCompC(0);

        uint16_t targetTime;
        targetTime = (time60deg*(30-TIMING))/60;

        // PI Controller
        int16_t fault;
        fault = (targetTime - measuredTime);

        static int32_t fault_I;
        fault_I = fault_I + fault;

        int16_t controllerOut;
        //controllerOut = (fault/P_DIVIDER + fault_I/I_DIVIDER);
        controllerOut = fault / P_DIVIDER;

        time60deg = time60deg - controllerOut;

        #ifdef MEASURE
        //bufferIn(pDataBuffer, 2030, 2030, 2030, 2030);
        //bufferIn(pDataBuffer, (int16_t)time60deg, (int16_t)measuredTime, fault, 0);
        #endif // MEASURE

        if(time60deg > MAX_T_60_DEG)
        {
            // too slow
            stopControlled();
        }
    }
}

void zeroCrossingListenerPhaseA(char edge)
{
    zeroCrossingCalculations();
}

void zeroCrossingListenerPhaseB(char edge)
{
    zeroCrossingCalculations();
}

void zeroCrossingListenerPhaseC(char edge)
{
    zeroCrossingCalculations();
}

void timeMeassurementOverflow(void)
{
    //logMsg("Timer overflow");
}
