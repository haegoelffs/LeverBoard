
#include <stdint.h>
#include <avr/interrupt.h>

#include "System/ringbufferDriveData.h"
#include "System/system.h"
#include "System/logger.h"

#include "controlled.h"
#include "drive.h"

BufferDriveData dataBuffer  = {{{0}}, 0, 0};
BufferDriveData *pDataBuffer = &dataBuffer;

int main(void)
{
    // initialization
    initUART();

    writeNewLine();
    logMsgLn("__Starting Leverboard V0.1__");

    initGPIOs();
    initTimers();
    initComp();
    initPWM();
    //initSPI();
    initAnalog();

    logMsgLn("Enable global interrupts...");
    sei();

    initDrive();

    setPowerLevel(50);


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

        /*spi_readStatusRegisters_BLOCKING();

        logNamedUnsignedInt("Status 1", getLastStatusRegister1Value(), 15);
        writeNewLine();
        logNamedUnsignedInt("Status 2", getLastStatusRegister2Value(), 15);
        writeNewLine();*/
    }

    return 0;
}

