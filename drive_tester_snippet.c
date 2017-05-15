#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/delay.h>
#include <stdlib.h>
#include <stdint.h>

#include "System/system.h"
#include "System/logger.h"
#include "System/loggerISR.h"

#define P_DIVIDER 100
#define I_DIVIDER 256

// functions
void proceedlater(void);

// variables
uint16_t target;
uint16_t actual;
int16_t controllOut;

int main(void)
{
    // initialization-------------------------------------------------------------------
    initUART();
    writeNewLine();
    writeNewLine();
    logMsg("__Starting__");

    logMsg("Enable global interrupts...");
    sei();

    initTimers();

    startAfterUs(100000, &proceedlater);

    target = 2000;
    actual = 1800;
    controllOut = 0;

    logNamedSignedInt("___controll out", controllOut, 30);
    logNamedSignedInt("___actual", actual, 30);

    while(1)
    {
        _delay_ms(10);
        writeBuffered();
    }

    return 0;
}

void proceedlater(void)
{
    static int16_t fault;
    fault = ((int16_t)(target - actual));

    static int32_t fault_I;
    fault_I += fault;

    //controllOut = (fault/P_DIVIDER + fault_I/I_DIVIDER);
    //controllOut = (fault_I/I_DIVIDER);
    controllOut = (fault/P_DIVIDER);

    actual = actual + controllOut;

    logSignedIntLineBuffered(actual, controllOut, fault, fault_I, 50);

    startAfterUs(100000, &proceedlater);
}
