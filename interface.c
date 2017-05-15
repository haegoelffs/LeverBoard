/*
last change: 08.5.2017
version: 0.1
*/

#include "system.h"
#include "interface.h"
#include <avr/io.h>
#include <util/delay.h>

void setNoBreakAlert()
	{
		setPiezoSound(1);
		//startTimeMeasurement(timeroverflow2);
	};
	
void quitNoBtreakAlert()
	{
		//uint16_t ms = 2001 getTime();
		if (1)
		{
			stopTimeMeasurement(timeroverflow2);
			setPiezoSound(0);
		}
	};

void setBatteryAlert()									//energy is the Battery voltage, it is a value from 1 to 4 (1 is very low, 4 is very good)
	{
		
		setPiezoSound(1);
		//startTimeMeasurement(timeroverflow2);
	};
	
void quitBatteryAlert()
	{
		//uint16_t ms = getTime2();
		if(1) 
		{
			//uint16_t a = stopTimeMeasurement2();
			setPiezoSound(0);
		}
	};
	
void flashLEDs()
{
	for(char i = 0; i < 10; ++i)
	setLEDsBatteryPower(4);
	_delay_ms(1000);
	setLEDsBatteryPower(0);
	
};

char give_new_dutycycle()
{
	char front = readInterfaceSensorsVoltageBLOCKING(0);	//Hallsensor front
	char tail = readInterfaceSensorsVoltageBLOCKING(1);;	   //Hallsensor tail
	char delta_sensors;
	char duty_cycletemp;
	if(front > tail)
	{
		delta_sensors = front - tail;
	}
	else delta_sensors = 0;
	if((delta_sensors < 25)) return 0;
	if (delta_sensors > 81) return 100;
	else
	{
		duty_cycletemp = (delta_sensors * 100)/81;
	}
	return duty_cycletemp;
}

void timeroverflow2()
{
	//do nothing
}