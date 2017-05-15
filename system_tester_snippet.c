#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/delay.h>
#include <stdlib.h>
#include <stdint.h>

#include "System/system.h"
#include "System/logger.h"
#include "System/loggerISR.h"



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

// functions
void zeroCrossingListenerPhaseA(char edge);
void zeroCrossingListenerPhaseB(char edge);
void zeroCrossingListenerPhaseC(char edge);

void proceedlater(void);


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
    initAnalog();

    //char test = readInterfaceSensorsVoltageBLOCKING(0);

    startAfterUs(10000, &proceedlater);
    logMsgBuffered("Test");


    writeBuffered();
    logMsgBuffered("Test2");
    logMsgBuffered("Test2");
    logMsgBuffered("Test2");
    logMsgBuffered("Test2");
    logMsg("write buffered");
    //writeBuffered();


    while(1)
    {
        _delay_ms(100);
        writeBuffered();
    }

    return 0;
}

void proceedlater(void)
{
    logMsgBuffered("proceed later test\r\n");
    startAfterUs(100000, &proceedlater);
}
