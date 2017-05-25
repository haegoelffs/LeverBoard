#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/delay.h>
#include <stdlib.h>
#include <stdint.h>
#include <util/delay.h>

#include "System/system.h"
#include "System/logger.h"
#include "startup.h"
#include "synchronize.h"
#include "System/ringbufferDriveData.h"

// variables
volatile uint8_t phasestate = 0;
volatile uint32_t time60deg = 0;

BufferDriveData dataBuffer  = {{{0}}, 0, 0};
BufferDriveData *pDataBuffer = &dataBuffer;

// functions
void startupFinishedCallback(uint8_t phasestate, uint16_t time60deg);

int main(void)
{
    // initialization-------------------------------------------------------------------
    initUART();
    initGPIOs();

    writeNewLine();
    logMsg("__Starting DRIVE tester__");

    logMsg("Enable global interrupts...");
    sei();

    logMsg("Enable bridge driver...");
    enableBridgeDriver(1);

    initTimers();
    initComp();
    initPWM();



    startSpeedUp(&startupFinishedCallback);

    while(1)
    {
        int16_t var1, var2, var3, var4;

        if(bufferOut(pDataBuffer,&var1, &var2, &var3 ,&var4))
        {
           logSignedInt(var1, 5);
            logMsg(" ");
            logSignedInt(var2, 5);
            logMsg(" ");
            logSignedInt(var3, 5);
            logMsg(" ");
            logSignedInt(var4, 5);
            writeNewLine();
        }
    }

    return 0;
}

void startupFinishedCallback(uint8_t phasestate, uint16_t time60deg)
{
    startSynchronize(phasestate, time60deg);
}
