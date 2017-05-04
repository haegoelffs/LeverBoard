#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "system.h"
#include "leverboardHardware.h"
#include "logger.h"


int main(void)
{
    initUART();
    logMsg("Starting system.c tester:");

    logMsg("Init GPIOs...");
    initGPIOs();

    logMsg("Init PWM...");
    initPWM();

    logMsg("Enable global interrupts...");
    sei();

    while(1)
    {
        enableBridgeDriver(1);
        _delay_ms(500);
        enableBridgeDriver(0);
        _delay_ms(500);
    }

    return 0;
}

