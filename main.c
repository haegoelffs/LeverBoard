/*
 */
#include <avr/io.h>
#include <stdint.h>

#include "drive.h"
#include "system.h"
#include "interface.h"
#include "energy.h"

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
	
    // init modules
    //initEnergy();
    //initInterface();
    //initDrive();
	

    while(1)
    {
		//Handle BatteryState
		char BatteryState = getBatteryState();
		switch(BatteryState) {
			case 0: switchPwmOnOff(0);
					if(numLed < 1) flashLEDs(1);
					numLed = 1;
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
		
		//get wished current
		signed char new_current = give_newcurrent(void);
		
	}
}
