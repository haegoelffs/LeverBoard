#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/delay.h>
#include <stdlib.h>
#include <stdint.h>

#include "System/system.h"
#include "System/ringbufferDriveData.h"
#include "System/logger.h"

#define P_DIVIDER ((int32_t)200)

// functions
void zeroCrossingListenerPhaseA(char edge);
void zeroCrossingListenerPhaseB(char edge);
void zeroCrossingListenerPhaseC(char edge);

void proceedlater(void);

// variables
BufferDriveData myBuffer = {{}, 0, 0};
BufferDriveData *pMyBuffer = &myBuffer;


int main(void)
{
    // initialization-------------------------------------------------------------------
    initUART();
    writeNewLine();
    writeNewLine();
    logMsgLine("__Starting SYSTEM tester__");

    logMsgLine("Enable global interrupts...");
    sei();

    initTimers();

    startAfterUs(10000, &proceedlater);


    while(1)
    {
        //_delay_ms(10);
        int16_t var1;
        int16_t *pVar1 = &var1;
        int16_t var2;
        int16_t *pVar2 = &var2;
        int16_t var3;
        int16_t *pVar3 = &var3;
        int16_t var4;
        int16_t *pVar4 = &var4;

        if(bufferOut(pMyBuffer,pVar1, pVar2, pVar3 ,pVar4))
        {
            logSignedInt(*pVar1, 5);
            logMsg(" ");
            logSignedInt(*pVar2, 5);
            logMsg(" ");
            logSignedInt(*pVar3, 5);
            logMsg(" ");
            logSignedInt(*pVar4, 5);
            writeNewLine();
        }
    }

    return 0;
}

void proceedlater(void)
{
    int32_t test = -40;
    int32_t test2 = test/P_DIVIDER;

    bufferIn(pMyBuffer,0,0,test,test2);
    startAfterUs(500000, &proceedlater);
}
