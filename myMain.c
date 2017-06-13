
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
        

		handle_batteryState(&numLed, &numPiezo, &systime);
		//emergencyShutDown(getLastPhaseACurrent());
		set_new_dutycycle();
    }

    return 0;
}

