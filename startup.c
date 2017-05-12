
#include "System/system.h"
#include "System/logger.h"
#include "startup.h"

#define GRADIENT_PERCENT_SPEED_UP ((uint8_t)2)
#define TIME_60DEG_SPEED_UP_START ((uint16_t)11000)
#define TIME_60DEG_SPEED_UP_END 2040

extern uint8_t phasestate;
extern uint16_t time60deg;

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

    setPWMDutyCycle(40);

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

    if(cnt >= 11)
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
