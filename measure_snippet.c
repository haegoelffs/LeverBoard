#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/delay.h>
#include <stdlib.h>

#include "System/system.h"
#include "System/logger.h"
#include <stdint.h>

#define TIMING ((uint8_t)10)

#define GRADIENT_PERCENT_SPEED_UP ((uint8_t)2)
#define TIME_60DEG_SPEED_UP_START ((uint16_t)11000)
#define TIME_60DEG_SPEED_UP_END 2040

#define NR_MEASURE_POINTS 50

typedef enum { no_meas, meas_t60deg, meas_zercrossing } State;
State state = no_meas;

volatile char phasestate = 0;
volatile uint32_t time60deg = 0;
volatile uint32_t timeRamp = 0;
volatile uint16_t systime_ms = 0;

volatile uint8_t measureNr = 0;
volatile uint8_t printResults = 0;

volatile uint8_t cnt = 0;

uint16_t t_60deg_measurepoints[NR_MEASURE_POINTS];
uint16_t t_zerocrossing_measurepoints[NR_MEASURE_POINTS];

// functions
void zeroCrossingListenerPhaseA(char edge);
void zeroCrossingListenerPhaseB(char edge);
void zeroCrossingListenerPhaseC(char edge);

void timeMeassurementOverflow(void);

void speedUpFrequenz(void);
void holdFrequenz(void);

void measureT60deg(void);
void measureZeroCrossing(void);

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
    initPWM();
    initSystime();

    registerVoltageZeroCrossingListenerPhaseA(&zeroCrossingListenerPhaseA);
    registerVoltageZeroCrossingListenerPhaseB(&zeroCrossingListenerPhaseB);
    registerVoltageZeroCrossingListenerPhaseC(&zeroCrossingListenerPhaseC);

    // start with ramp------------------------------------------------------------------
    logMsg("Start engine...");
    setPWMDutyCycle(30);

    time60deg = TIME_60DEG_SPEED_UP_START; //us
    speedUpFrequenz();

    _delay_ms(2000);
    _delay_ms(2000);
    _delay_ms(2000);
    _delay_ms(2000);

    // measure---------------------------------------------------------------------------
    setPWMDutyCycle(28);
    logMsg("28...");
    _delay_ms(500);
    setPWMDutyCycle(27);
    logMsg("27...");
    _delay_ms(500);
    setPWMDutyCycle(26);
    logMsg("26...");
    _delay_ms(500);
    setPWMDutyCycle(25);
    logMsg("25...");


    logMsg("Start measurement time 60deg...");
    state = meas_t60deg;

    while(1)
    {
        if(printResults == 1)
        {
            state = no_meas;
            stopTimeMeasurement();

            measureNr = 0;
            while(measureNr <= NR_MEASURE_POINTS)
            {
                logNamedUnsignedInt("t_60deg", t_60deg_measurepoints[measureNr], 20);
                measureNr++;
            }
            writeNewLine();
            writeNewLine();
            writeNewLine();

            // reset
            logMsg("Start measurement time zerocrossing...");
            measureNr = 0;
            printResults = 3; // do not print
            state = meas_zercrossing;
        }

        if(printResults == 2)
        {
            measureNr = 0;
            while(measureNr <= NR_MEASURE_POINTS)
            {
                logNamedUnsignedInt("t_zeroc", t_zerocrossing_measurepoints[measureNr], 20);
                measureNr++;
            }
            writeNewLine();
            writeNewLine();
            writeNewLine();

            // reset
            stopTimeMeasurement();
            state = no_meas;
            printResults = 3;
        }
    }

    return 0;
}

void holdFrequenz(void)
{
    switchPhase();

    startAfterUs(time60deg, &holdFrequenz);

    switch(state)
    {
        case no_meas:
        break;

        case meas_t60deg:
        measureT60deg();
        break;

        case meas_zercrossing:
        measureZeroCrossing();
        break;
    }
}

void speedUpFrequenz(void)
{
    switchPhase();

    cnt++;

    if(cnt >= 11)
    {
        time60deg = (TIME_60DEG_SPEED_UP_START - timeRamp*GRADIENT_PERCENT_SPEED_UP/100);
        timeRamp = timeRamp + time60deg;
        cnt = 0;
    }

    //logUnsignedInt(timeRamp, 20);
    //logUnsignedInt(time60deg, 20);

    if(time60deg < TIME_60DEG_SPEED_UP_END)
    {
        //logMsg("finito");
        startAfterUs(time60deg, &holdFrequenz);
    }
    else
    {
        startAfterUs(time60deg, &speedUpFrequenz);
    }
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

void measureT60deg(void)
{
    if(isTimeMeasurementRunning())
    {
        t_60deg_measurepoints[measureNr] = (uint16_t)stopTimeMeasurement();
        measureNr++;
    }
    else
    {
        startTimeMeasurement(&timeMeassurementOverflow);
    }

    if(measureNr >= NR_MEASURE_POINTS)
    {
        printResults = 1;
    }
    else
    {
        startAfterUs(time60deg, &measureT60deg);
    }
}

void measureZeroCrossing(void)
{
    if(!isTimeMeasurementRunning())
    {
        startTimeMeasurement(&timeMeassurementOverflow);
    }
}



void zeroCrossingListenerPhaseA(char edge)
{
    setEnableCompA(0); // disable isr -> interrupt only on first zero crossing

    if(state == meas_zercrossing && isTimeMeasurementRunning())
    {
        if(measureNr >= NR_MEASURE_POINTS)
        {
            printResults = 2;
        }
        else
        {
            t_zerocrossing_measurepoints[measureNr] = (uint16_t)stopTimeMeasurement();
            measureNr++;
        }
    }
}

void zeroCrossingListenerPhaseB(char edge)
{
    setEnableCompB(0); // disable isr -> interrupt only on first zero crossing

    if(state == meas_zercrossing && isTimeMeasurementRunning())
    {
        if(measureNr >= NR_MEASURE_POINTS)
        {
            printResults = 2;
        }
        else
        {
            t_zerocrossing_measurepoints[measureNr] = (uint16_t)stopTimeMeasurement();
            measureNr++;
        }
    }
}

void zeroCrossingListenerPhaseC(char edge)
{
    setEnableCompC(0); // disable isr -> interrupt only on first zero crossing

    if(state == meas_zercrossing && isTimeMeasurementRunning())
    {
        if(measureNr >= NR_MEASURE_POINTS)
        {
            printResults = 2;
        }
        else
        {
            t_zerocrossing_measurepoints[measureNr] = (uint16_t)stopTimeMeasurement();
            measureNr++;
        }
    }
}

void timeMeassurementOverflow(void)
{
    logMsg("Timer ERROR");
}
