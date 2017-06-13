#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdint.h>

#include "System/system.h"
#include "System/logger.h"
#include "fixCommutated.h"
#include "controlled.h"
#include "freeRunning.h"
#include "System/ringbufferDriveData.h"
#include "drive.h"

#define MAX_PWR_LEVEL 42

//#define MEASURE

// variables
uint8_t phasestate = 0;
uint32_t time60deg = 0;
int8_t powerlevel = 0;

static DriveState state = free_running;

// internal functions

// callback functions
void startupStopedCallback(uint8_t phasestate, uint16_t time60deg);
void measurementDataAvailableCallback(char phaseLastMeasurement);
void controlledStopedCallback();
void freeRunningStopedCallback(void);

void initDrive(void)
{
    logMsgLn("init drive...");

    registerMeasurementDataAvailableListener(&measurementDataAvailableCallback);
    startFreeRunning(&freeRunningStopedCallback);
}

void setPowerLevel(int8_t newPowerlevel)
{
    switch(state)
        {
            case free_running:
            if(newPowerlevel > 0)
            {
                stopFreeRunning();
            }

            break;

            case fix_commutated:
            if(newPowerlevel == 0)
            {
                powerlevel = 0;
                stopSpeedUp();
            }
            break;

            case controlled:
            powerlevel = newPowerlevel;
            break;
        }
}

void measurementDataAvailableCallback(char phaseLastMeasurement)
{
    static char duty_cycle = 0;
    int8_t actual_current = getLastPhaseACurrent();

    #ifdef MEASURE
    bufferIn(pDataBuffer, actual_current, getLastBattery(), phaseLastMeasurement, duty_cycle);
    #endif // MEASURE

    if(((actual_current > powerlevel) && (duty_cycle > 0)) || actual_current > 85)
    {
        --duty_cycle;
    }
    else if ((actual_current < powerlevel && (duty_cycle < 100)) || actual_current < 85)
    {
        ++duty_cycle;
    }
    setPWMDutyCycle(duty_cycle);
}

void freeRunningStopedCallback(void)
{
    powerlevel = MAX_PWR_LEVEL; // max powerlevel for startup
    startSpeedUp(&startupStopedCallback);
}

void startupStopedCallback(uint8_t phasestate, uint16_t time60deg)
{
    if(powerlevel == 0)
    {
        state = free_running;
        startFreeRunning(&freeRunningStopedCallback);
    }
    else
    {
        state = controlled;
        startControlled(phasestate, time60deg+100, &controlledStopedCallback);
    }
}

void controlledStopedCallback()
{
    powerlevel = 0;
    state = free_running;
    startFreeRunning(&freeRunningStopedCallback);
}
