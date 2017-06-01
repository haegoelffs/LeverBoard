#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>
#include <stdlib.h>
#include <stdint.h>

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
//void measurementDataAvailable();

int main(void)
{
    // initialization-------------------------------------------------------------------
    initUART();
    writeNewLine();
    logMsgLn("__Starting DRIVE tester__");

    initGPIOs();
    initTimers();
    initComp();
    initPWM();
    //initSPI();
    //initAnalog();

    logMsgLn("Enable global interrupts...");
    sei();

    logMsgLn("Enable bridge driver...");
    enableBridgeDriver(1);
    setDC_cal(1);

    setPowerLED();
    setLEDsBatteryPower(2);

    //registerMeasurementDataAvailableListener(&measurementDataAvailable);

    #ifdef MEASURE
    logMsgLn("Drive in measure mode");
    #endif // MEASURE

    startSpeedUp(&startupFinishedCallback);

    while(1)
    {
        #ifdef MEASURE
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
        #endif // MEASURE

        /*spi_readStatusRegisters_BLOCKING();

        logNamedUnsignedInt("Status 1", getLastStatusRegister1Value(), 15);
        writeNewLine();
        logNamedUnsignedInt("Status 2", getLastStatusRegister2Value(), 15);
        writeNewLine();*/
    }

    return 0;
}

void startupFinishedCallback(uint8_t phasestate, uint16_t time60deg)
{
    startSynchronize(phasestate, time60deg);
}

/*void measurementDataAvailable()
{
    bufferIn(pDataBuffer, getLastPhaseACurrent(), getLastPhaseBCurrent(), getLastPhaseCCurrent(), getLastBattery());
}*/
