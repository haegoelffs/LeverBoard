#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/delay.h>
#include <stdlib.h>

#include "System/system.h"
#include "System/logger.h"
#include <stdint.h>

#define TIMING ((uint8_t)10)

#define SPEED_UP_STEP 1000
#define SPEED_UP_START_T60DEGREES 262144 // max. value function "startAfter()"
#define SPEED_UP_END_T60DEGREES 2040

/** calculation:
V_max = 30km/h = 8.3m/s
V_min = 3.6km/h = 1m/s
D_rad = 82mm
i_riemen = 3:1
i_motor = 7:1

Min:
f_rad,min = V_min/(D_rad*PI) = 3.88Hz
f_motor,min = f_rad,min * i_riemen = 11.65Hz
f_el,min = f_motor,min * i_motor = 81.52Hz
T_el,max = 1/f_el,min = 12.27ms
T_el60deg,max = T_el,max/6 = 2.04ms

Max:
f_rad,max = V_max/(D_rad*PI) = 32.22Hz
f_motor,max = f_rad,max * i_riemen = 96.66Hz
f_el,max = f_motor,max * i_motor = 676.60Hz
T_el,min = 1/f_el,max = 1.48ms
T_el60deg,min = T_el,min/6 = 246.33us

Range T_el60deg: 240us ... 2'040us

*/

volatile char phasestate = 0;
volatile uint16_t time60deg = 0;

// functions
void zeroCrossingListenerPhaseA(char edge);
void zeroCrossingListenerPhaseB(char edge);
void zeroCrossingListenerPhaseC(char edge);

void timeMeassurementOverflow(void);

void speedUpFrequenz(void);
void holdFrequenz(void);
void switchPhase(void);

int main(void)
{
    // initialization-------------------------------------------------------------------
    initUART();
    writeNewLine();
    writeNewLine();
    logMsg("__Starting system.c tester__");

    logMsg("Enable global interrupts...");
    sei();

    initTimers();
    initComp();

    registerVoltageZeroCrossingListenerPhaseA(&zeroCrossingListenerPhaseA);
    registerVoltageZeroCrossingListenerPhaseB(&zeroCrossingListenerPhaseB);
    registerVoltageZeroCrossingListenerPhaseC(&zeroCrossingListenerPhaseC);

    initPWM();

    setPWMDutyCycle(20);

    setPWMDutyCycle(80);

    setPWMDutyCycle(120);

    while(1)
    {
        //writeBuffered();
    }

    return 0;
}

void switchPhase(void)
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

void speedUpFrequenz(void)
{
    switchPhase();

    time60deg = time60deg - SPEED_UP_STEP;

    if(time60deg < SPEED_UP_END_T60DEGREES)
    {
        startAfterUs(time60deg, &speedUpFrequenz);
    }
    else
    {
        startAfterUs(time60deg, &holdFrequenz);
    }
}

void holdFrequenz(void)
{
    phasestate = (phasestate+1)%6;
    changePhaseState(phasestate);

    startAfterUs(time60deg, &holdFrequenz);
}

void zeroCrossingListenerPhaseA(char edge)
{
    // measure current frequenzy
    if(isTimeMeasurementRunning())
    {
        time60deg = stopTimeMeasurement();
    }
    else
    {
        startTimeMeasurement(&timeMeassurementOverflow);
    }

    // switch phase
    // Range T_el60deg: 240us ... 2'040us

    // t_next_phase = time60deg/60*(30-TIMING)
    // Range t_next_phase: 4 ... 1.02e3
    // Rechnung grösster Wert: time60deg*30 = 61.2e3 --> 16bit reicht

    uint16_t temp = time60deg*(30-TIMING)/60;
    //startAfterUs(temp, &timerCall);
}

void zeroCrossingListenerPhaseB(char edge)
{
    setEnableCompB(0); // disable isr -> interrupt only on first zero crossing

    if(isTimeMeasurementRunning())
    {
        time60deg = stopTimeMeasurement();
    }
    else
    {
        startTimeMeasurement(&timeMeassurementOverflow);
    }

    uint16_t temp = time60deg*(30-TIMING)/60;

    //startAfterUs(temp, &timerCall);
}

void zeroCrossingListenerPhaseC(char edge)
{
    setEnableCompC(0); // disable isr -> interrupt only on first zero crossing

    if(isTimeMeasurementRunning())
    {
        time60deg = stopTimeMeasurement();
    }
    else
    {
        startTimeMeasurement(&timeMeassurementOverflow);
    }

    uint16_t temp = time60deg*(30-TIMING)/60;
    //startAfterUs(temp, &timerCall);
}

void timeMeassurementOverflow(void)
{

}
