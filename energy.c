/*
last change: 08.5.2017
version: 0.1
*/
#include "system.h"
#include "energy.h"
#include <avr/io.h>

void emergencyShutDown(char current)
{
	if(current > 47)
	{
		switchPwmOnOff(0);
		setNoBreakAlert();
	}
}

char getBatteryState()
{
	char voltage = readBatteryVoltageBlocking();
	if(voltage <= 90)     //21V // 1.75
	{
		return 0;
	}
	else if(voltage <= 94) //22V //1.833333
	{
		 return 1;		
	}
	else if (voltage <= 96) //22.5V //1.87 V
	{
		return 2;
	}
	else if (voltage < 104) 
	{
		return 3;       //24.5 V // 2.041666
	}
	else return 4;	    //smaller/equal  25 V //2.08 V

}

void handle_batteryState(char* numLed, char* numPiezo, uint16_t* systime)
{
	char BatteryState = getBatteryState();
	logUnsignedInt("BatteryState",BatteryState,30);
	switch(BatteryState) {
		case 0: switchPwmOnOff(0);
		if(*numLed < 1)
		{
			flashLEDs();
			++(*numLed) ;
		}
		*numPiezo = 0;
		setLEDsBatteryPower(0);
		break;
		case 1: switchPwmOnOff(1);
		if (numPiezo< 1)
		{
			systime = setBatteryAlert();
			++(*numPiezo);
		}
		else quitBatteryAlert(systime);
		numLed = 0;
		setLEDsBatteryPower(1);
		break;
		case 2: switchPwmOnOff(1);
		*numLed = 0;
		*numPiezo = 0;
		setLEDsBatteryPower(2);
		break;
		case 3: switchPwmOnOff(1);
		*numLed = 0;
		*numPiezo = 0;
		setLEDsBatteryPower(3);
		break;
		case 4: switchPwmOnOff(1);
		*numLed = 0;
		setLEDsBatteryPower(4);
		if (*numPiezo < 1)
		{
			*systime= setNoBreakAlert();
		}
		else quitNoBtreakAlert(*systime);
	}
}