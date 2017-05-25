
#include "System/system.h"
#include "System/logger.h"
#include "startup.h"

#define GRADIENT_PERCENT_SPEED_UP ((uint8_t)2)
//#define TIME_60DEG_SPEED_UP_START ((uint16_t)11000)
#define TIME_60DEG_SPEED_UP_START ((uint16_t)8000)
//#define TIME_60DEG_SPEED_UP_END 2040
#define TIME_60DEG_SPEED_UP_END 2000
#define STEPSIZE 5

uint8_t phasestate;
uint16_t time60deg;

static volatile uint32_t timeRamp = 0;
static volatile uint8_t cnt = 0;

void (*callback)(uint8_t, uint16_t);

// function declarations
void speedUpFrequenz(void);
void switchPhase(void);

void startSpeedUp(void (*startupFinishedCallback)(uint8_t, uint16_t))
{
    logMsg("Start speed up engine...");
    callback = startupFinishedCallback;

    setEnableCompA(0);
    setEnableCompB(0);
    setEnableCompC(0);

    setPWMDutyCycle(50);

    time60deg = TIME_60DEG_SPEED_UP_START; //us
    speedUpFrequenz();
}

void switchPhase(void)
{
    phasestate = (phasestate+1)%6;
    changePhaseState(phasestate);
}

void speedUpFrequenz(void)
{
    switchPhase();

    cnt++;

    if(cnt >= STEPSIZE)
    {
        time60deg = (TIME_60DEG_SPEED_UP_START - timeRamp*GRADIENT_PERCENT_SPEED_UP/100);
        timeRamp = timeRamp + time60deg;
        cnt = 0;
    }

    if(time60deg < TIME_60DEG_SPEED_UP_END)
    {
        callback(phasestate, time60deg);
    }
    else
    {
        startAfterUs(time60deg, &speedUpFrequenz);
    }
}

/*void zeroCrossingListenerPhaseA(char edge)
{
    setEnableCompA(0);

    if(isTimeMeasurementRunning())
    {
        uint16_t measuredTime = (uint16_t)stopTimeMeasurement();
    }

    setEnableCompC(1);
}

void zeroCrossingListenerPhaseB(char edge)
{
    setEnableCompB(0);
    setEnableCompA(1);
}

void zeroCrossingListenerPhaseC(char edge)
{
    setEnableCompC(0);
    setEnableCompB(1);
}

avrdude: safemode: hfuse reads as 99
avrdude: safemode: efuse reads as FF

avrdude: safemode: hfuse reads as 99
avrdude: safemode: efuse reads as FF
avrdude: safemode: Fuses OK (E:FF, H:99, L:62)

avrdude: safemode: hfuse reads as D8
avrdude: safemode: efuse reads as FD

avrdude: safemode: hfuse reads as D8
avrdude: safemode: efuse reads as FD
avrdude: safemode: Fuses OK (E:FD, H:D8, L:FF)
avrdude: safemode: Fuses OK (E:FD, H:D8, L:FF)
avrdude: safemode: Fuses OK (E:FD, H:D8, L:FF)


avrdude -c dragon_isp -P usb -p atmega2560 -U lfuse:w:0xFF:m -U hfuse:w:0xD8:m -U efuse:w:0xFD:m



*/
