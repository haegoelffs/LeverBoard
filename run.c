
#include "System/system.h"
#include "System/logger.h"
#include "synchronize.h"
#include "System/ringbufferDriveData.h"

#include <stdint.h>

#define TIMING 5
#define P_DIVIDER 200
//#define P_DIVIDER 128
#define I_DIVIDER 256

 static uint8_t phasestate;
 static uint16_t time60deg;

 extern BufferDriveData *pDataBuffer;

// functions
void zeroCrossingListenerPhaseA(char edge);
void zeroCrossingListenerPhaseB(char edge);
void zeroCrossingListenerPhaseC(char edge);

void timeMeassurementOverflow(void);

void switchPhases(void);
void switchPhaseAndManageComps(void);

void startSynchronize(uint8_t phasestate_init, uint16_t time60deg_init)
{
    phasestate = phasestate_init;
    time60deg = time60deg_init;

    registerVoltageZeroCrossingListenerPhaseA(&zeroCrossingListenerPhaseA);
    registerVoltageZeroCrossingListenerPhaseB(&zeroCrossingListenerPhaseB);
    registerVoltageZeroCrossingListenerPhaseC(&zeroCrossingListenerPhaseC);

    setPWMDutyCycle(20);

    switchPhases();
}

void switchPhases(void)
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

    //startMeasureProcedure(); // temp

    if(isTimeMeasurementRunning())
    {
        stopTimeMeasurement();
    }
    startTimeMeasurement(&timeMeassurementOverflow);

    startAfterUs(time60deg, &switchPhases);
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
        bufferIn(pDataBuffer, time60deg, measuredTime, fault, controllerOut);
        #endif // MEASURE
    }
}

void zeroCrossingListenerPhaseA(char edge)
{
    //setEnableCompA(0); // disable isr -> interrupt only on first zero crossing
    zeroCrossingCalculations();
}

void zeroCrossingListenerPhaseB(char edge)
{
    //setEnableCompB(0); // disable isr -> interrupt only on first zero crossing
    zeroCrossingCalculations();
}

void zeroCrossingListenerPhaseC(char edge)
{
    //setEnableCompC(0); // disable isr -> interrupt only on first zero crossing
    zeroCrossingCalculations();
}

void timeMeassurementOverflow(void)
{
    //logMsg("Timer overflow");
}
