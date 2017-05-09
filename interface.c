/*
last change: 08.5.2017
version: 0.1
*/

#include "system.h"
#include "interface.h"
#include <avr/io.h>
#include <util/delay.h>
/*
void initInterface()
		{
			return;
		};
*/
void setNoBreakAlert()
	{
		setPiezoSound(1);
		startTimeMeasurement2(timeroverflow2);
	};
	
void quitNoBtreakAlert()
	{
		uint16_t ms = getTime2();
		if (ms > 2000)
		{
			stopTimeMeasurement2(timeroverflow2);
			setPiezoSound(0);
		}
	};

void setBatteryAlert()									//energy is the Battery voltage, it is a value from 1 to 4 (1 is very low, 4 is very good)
	{
		
		setPiezoSound(1);
		startTimeMeasurement2(timeroverflow2);
	};
	
void quitBatteryAlert()
	{
		uint16_t ms = getTime2();
		if (ms > 1000) 
		{
			uint16_t a = stopTimeMeasurement2();
			setPiezoSound(0);
		}
	};
	


void setEnergyLEDs(char energy)				//energy is the Battery voltage, it is a value from 0 to 4 (0 is very low, 4 is very good)
	{
		setLEDsBatteryPower(energy);
	};
	
void flashLEDs()
{
	for(char i = 0; i < 10; ++i)
	setLEDsBatteryPower(4);
	_delay_ms(1000);
	setLEDsBatteryPower(0);
	
};

signed char give_newcurrent(void);			//gets hallvoltage returns wished power (MAX POWER 42.5 A)
	{
		char front = readInterfaceSensorsVoltage(0);	//Hallsensor front
		char tail = readInterfaceSensorsVoltage(1);	   //Hallsensor tail
		char delta_sensors;	
		if(front > tail) 
		{
			delta_sensors = front - tail;
		}
		else delta_sensors = tail - front;
		if((delta_sensors < 51))
		{
			return (signed char)0;
		}
		else if (delta_sensors > 255) return (signed char 42);
		else return (signed char)((42 * delta_sensors)/255);			
		
	};


void timeroverflow2()
{
	//do nothing
}