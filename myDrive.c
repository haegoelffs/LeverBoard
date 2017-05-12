#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/delay.h>
#include <stdlib.h>
#include <stdint.h>

#include "System/system.h"
#include "System/logger.h"
#include "startup.h"
#include "synchronize.h"

// variables
volatile uint8_t phasestate = 0;
volatile uint32_t time60deg = 0;

// functions
void startupFinishedCallback(uint8_t phasestate, uint16_t time60deg);

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

    startSpeedUp(&startupFinishedCallback);

    while(1)
    {
    }

    return 0;
}

void startupFinishedCallback(uint8_t phasestate, uint16_t time60deg)
{
    startSynchronize(phasestate, time60deg);
}
