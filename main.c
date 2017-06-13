#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdint.h>

#include "drive.h"
#include "system.h"
#include "interface.h"
#include "energy.h"
#include "logger.h"
#include <avr/interrupt.h>
#include <util/delay.h>


//Global Variables
uint16_t delta_time = 0;
char enable = 1;

int main(void)
{
	initGPIOs();


	//init variables

	char phaseState;
	char numPiezo = 0;				//number of times piezo made a noise
	char numLed = 0;				//number of times LEDs were flashed
	char actual_current = 0;
	uint16_t systime = 0;

	// init modules and Hardware
    initINTERFACE();
	initUART();
	initAnalog();
	initPWM();
	initComp();
	sei();					//enable interrupts
	initTimers();
    initDrive();
	initSystime();


    while(1)
    {


		phaseState = getPhaseState();
		handle_batteryState(&numLed, &numPiezo, &systime);
		emergencyShutDown(actual_current);
		switch (phaseState)
		{
			case 0: actual_current= getLastPhaseACurrent();
					break;
			case 1: actual_current= getLastPhaseCCurrent();
					break;
			case 2: actual_current= getLastPhaseBCurrent();
					break;
            case 3: actual_current= getLastPhaseACurrent();
					break;
			case 4: actual_current= getLastPhaseCCurrent();
					break;
			case 5: actual_current= getLastPhaseBCurrent();
					break;
		}
		set_new_dutycycle();

	}
}

