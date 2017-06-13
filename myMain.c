
#include <stdint.h>
#include <avr/interrupt.h>

#include "System/ringbufferDriveData.h"
#include "System/system.h"
#include "System/logger.h"

#include "drive.h"
#include "interface.h"
#include "energy.h"

BufferDriveData dataBuffer  = {{{0}}, 0, 0};
BufferDriveData *pDataBuffer = &dataBuffer;

int main(void)
{
    // init system
    initUART();

    writeNewLine();
    logMsgLn("__Starting Leverboard V0.1__");

    initGPIOs();
    initTimers();
    initComp();
    initPWM();
    //initSPI();
    initAnalog();
    initSystime();


    logMsgLn("Enable global interrupts...");
    sei();

    // init modules
    initDrive();
    initINTERFACE();

    // variables
	char numPiezo = 0;				//number of times piezo made a noise
	char numLed = 0;				//number of times LEDs were flashed
	uint16_t systime = 0;

    while(1)
    {
        /*int16_t var1, var2, var3, var4;

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
        }*/

        /*spi_readStatusRegisters_BLOCKING();

        logNamedUnsignedInt("Status 1", getLastStatusRegister1Value(), 15);
        writeNewLine();
        logNamedUnsignedInt("Status 2", getLastStatusRegister2Value(), 15);
        writeNewLine();*/

		handle_batteryState(&numLed, &numPiezo, &systime);
		//emergencyShutDown(getLastPhaseACurrent());
		set_new_dutycycle();
    }

    return 0;
}

