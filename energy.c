/*
last change: 08.5.2017
version: 0.1
*/
#include "system.h"
#include "energy.h"
#include <avr/io.h>

/*
void initEnergy()
{
return;
};*/

char getBatteryState()
{
	char voltage = readBatteryVoltage();
	if(voltage <= 21) 
	{
		return 0;
	}
	else if(voltage <= 220) //22V
	{
		 return 1;		
	}
	else if (voltage <= 225) //22.5V
	{
		return 2;
	}
	else if (voltage < 250) 
	{
		return 3;       //24.5 V
	}
	else return 4;	    //bigger/equal  25 V*/

}
