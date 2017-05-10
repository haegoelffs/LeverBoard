
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
#include "global.h"     //Global variables

int main(void)
{
    // init Hardware
    initUART();
	initGPIOs();
    initAnalog();
    initPWM();
	
	
	//init variables
	
	char numPiezo = 0;       //number of times piezo made a noise
	char numLed = 0;		//number of times LEDs were flashed
	signed char new_current;
	char duty_cycle = 0;
	phaseState = 7;
	uint16_t delta_time = 100;
	
	
    // init modules
    //initEnergy();
    //initInterface();
    initDrive();
	
	//Set Power LED
	setPowerLED();
	
    while(1)
    {
		//Handle BatteryState
		char BatteryState = getBatteryState();
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
						setBatteryAlert();
						++numPiezo;		
					}
					else quitBatteryAlert();
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
						setNoBreakAlert();
					}
					else quitNoBtreakAlert();
		}
		
		
		//signed char new_current = give_newcurrent();
		duty_cycle= give_new_dutycycle();
		char actual_current = give_actualcurrent(phase_state);
		duty_cycle = setPWMDutyCycle_dr(duty_cycle, actual_current);
		//rise_sink_pwm_dutyc(new_current,actual_current,duty_cycle);
		emergencyShutDown(actual_current);
	}
}
