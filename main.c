
/*
last change: 08.5.2017
version: 0.1
*/
#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdint.h>

#include "drive.h"
#include "system.h"
#include "interface.h"
#include "energy.h"
#define EXTERN
//#include "global.h"     //Global variables
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
		
		/*
		//Handle BatteryState
		char BatteryState = getBatteryState();
		logUnsignedInt("BatteryState",BatteryState,30);
		switch(BatteryState) {
			case 0: switchPwmOnOff(0);
					if(numLed < 1)
					{
						flashLEDs();
						++numLed ;
					}
					numPiezo = 0;
					setLEDsBatteryPower(0);
					break;
			case 1: switchPwmOnOff(1);
					if (numPiezo< 1)
					{
						systime = setBatteryAlert();
						++numPiezo;		
					}
					else quitBatteryAlert(systime);
					numLed = 0;
					setLEDsBatteryPower(1);
					
					break;
			case 2: switchPwmOnOff(1);
					numLed = 0;
					numPiezo = 0;
					setLEDsBatteryPower(2);
					break;
			case 3: switchPwmOnOff(1);
					numLed = 0;
					numPiezo = 0;
					setLEDsBatteryPower(3);
					break;
			case 4: switchPwmOnOff(1);
					numLed = 0;
					setLEDsBatteryPower(4);
					if (numPiezo < 1)
					{
						systime= setNoBreakAlert();
					}
					else quitNoBtreakAlert(systime);
		}*/
		
		phaseState = getPhaseState();
		handle_batteryState(&numLed, &numPiezo, &systime);
		emergencyShutDown(actual_current);
		switch (phaseState)
		{
			case 1: actual_current= getLastPhaseACurrent();
					break;
			case 2: actual_current= getLastPhaseBCurrent();
					break;
			case 3: actual_current= getLastPhaseCCurrent();
					break;
		}
		set_new_dutycycle();
		
	}
}
		
