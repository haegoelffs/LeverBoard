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
