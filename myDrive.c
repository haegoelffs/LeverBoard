#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdint.h>

#include "System/system.h"
#include "System/logger.h"
#include "fixCommutated.h"
#include "controlled.h"
#include "System/ringbufferDriveData.h"
#include "drive.h"

#define MEASURE

#define P_DIVIDER 128
#define I_DIVIDER 256

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

void initDrive(void)
{
    logMsgLn("init drive...");
    registerMeasurementDataAvailableListener(&measurementDataAvailableCallback);
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
                // start with full power
                setPWMDutyCycle(25);
                enableBridgeDriver(1);
                startSpeedUp(&startupFinishedCallback);
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

    #ifdef MEASURE
    int8_t actual_current = 0;
    switch(phaseLastMeasurement)
    {
        case 'A':
        actual_current = getLastPhaseACurrent();
        break;

        case 'B':
        actual_current = getLastPhaseBCurrent();
        break;

        default:
        break;
    }

    bufferIn(pDataBuffer, actual_current, getLastBattery(), phaseLastMeasurement, duty_cycle);
    #endif // MEASURE

    actual_current = (128-actual_current);
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
