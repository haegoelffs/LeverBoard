
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
#include "global.h"     //Global variables
#include "logger.h"
#include <avr/interrupt.h>
#include <util/delay.h>


//Global Variables
char phaseState = 1;
uint16_t delta_time = 0;
char enable = 1;

int main(void)
{   
	initGPIOs();


	//init variables
	
	char numPiezo = 0;				//number of times piezo made a noise
	char numLed = 0;				//number of times LEDs were flashed
	//signed char new_current;		
	char duty_cycle = 0;
	int tempo = 0;				//For forcing;			
	char actual_current;
	uint16_t systime = 0;
	
	// init modules and Hardware
							
	//setPowerLED();		//Set Power LED
	initUART();
	initAnalog();
	changePhaseState(phaseState);
	initPWM();
	initComp();
	sei();					//enable interrupts
	initTimers();		
    initDrive();
	initSystime();
	
	
	// systime////////////
	
	uint16_t getSystimeMs() = ;
	/////////////
	
    while(1)
    {
		
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
		}
		emergencyShutDown(actual_current);
		actual_current = give_actualcurrent(phaseState);
		char duty_cycle_prov = give_new_dutycycle();
		duty_cycle = setPWMDutyCycle_dr(duty_cycle_prov, actual_current);
	
		
	}
}
		
