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

//#define MEASURE

// variables
uint8_t phasestate = 0;
uint32_t time60deg = 0;
int8_t powerlevel = 0;

static DriveState state = free_running;

// internal functions
static void calculatePower(void);

// callback functions
void startupFinishedCallback(uint8_t phasestate, uint16_t time60deg);
void measurementDataAvailableCallback(char phaseLastMeasurement);
void tooSlowForControlledCallback();
void freeRunningStopedCallback(void);

void initDrive(void)
{
    logMsgLn("init drive...");

    //registerMeasurementDataAvailableListener(&measurementDataAvailableCallback);
    startFreeRunning(&freeRunningStopedCallback);
}

DriveState getDriveState()
{
    return state;
}

void setPowerLevel(int8_t newPowerlevel)
{
    switch(state)
        {
            case free_running:
            if(newPowerlevel > 0)
            {
                stopFreeRunning();
                // start with full power
                // setPWMDutyCycle(25);
                // enableBridgeDriver(1);
                // startSpeedUp(&startupFinishedCallback);
            }

            break;

            case fix_commutated:
            break;

            case controlled:
            powerlevel = newPowerlevel;
            break;
        }
}

static void calculatePower(void)
{
}

void startupFinishedCallback(uint8_t phasestate, uint16_t time60deg)
{
    state = controlled;
    setPWMDutyCycle(45);
    startControlled(phasestate, time60deg+100, &tooSlowForControlledCallback);
}

void measurementDataAvailableCallback(char phaseLastMeasurement)
{
    static char duty_cycle = 35;
    int8_t actual_current = getLastPhaseACurrent();

    #ifdef MEASURE
    bufferIn(pDataBuffer, actual_current, getLastBattery(), phaseLastMeasurement, duty_cycle);
    #endif // MEASURE

	char tobe_current = 50;

    if(((actual_current > tobe_current) && (duty_cycle > 0)) || actual_current > 85)
		{
			--duty_cycle;
		}
		else if ((actual_current < tobe_current && (duty_cycle < 100)) || actual_current < 85)
		{
			++duty_cycle;
		}
		setPWMDutyCycle(duty_cycle);
}

void tooSlowForControlledCallback()
{
    // leave controlled
    stopControlled();
    enableBridgeDriver(0);
    setPWMDutyCycle(0);
    state = free_running;
}

void freeRunningStopedCallback(void)
{

}
