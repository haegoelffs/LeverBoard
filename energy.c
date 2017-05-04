
/*
last change: 18.4.2017
version: 0.1
*/


#include "system.h"
#include "energy.h"
#include <avr/io.h>

void initEnergy()
{
return;
};

char getBatteryState()
{
	char BatteryState;
	char voltage = readBatteryVoltage();
	if (voltage <= 21)
		{
			BatteryState = 0;
		}
	else if (voltage <= 220)        //22V
		{
			BatteryState = 1;
		}
	else if (voltage <= 225)        //22.5V
	{
		BatteryState = 2;
	}
	else if (voltage < 250)        //24.5 V
	{
		BatteryState = 3;
	}
	else
		BatterState = 4;            //bigger/eqaul  25 V
	return BatteryState;
};
