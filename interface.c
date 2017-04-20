/*
last change: 20.4.2017
version: 1.2
*/


#include system.h
#include <avr/io.h>

void initInterface()
		{
			return;
		};

void setNoBreakAlert()
	{
		setPiezoSound(1);
		_delay_ms(1000);    
	};
	
void quitNoBtreakAlert()
	{
		setPiezoSound(1);   
	};

void setBatteryAlert()									//energy is the Battery voltage, it is a value from 1 to 4 (1 is very low, 4 is very good)
	{
		setPiezoSound(1);
		_delay_ms(2000);    
	};
	
void quitBatteryAlert()
	{
		setPiezoSound(0);
	};
	


void setEnergyLEDs(unsigned char energy);				//energy is the Battery voltage, it is a value from 0 to 4 (0 is very low, 4 is very good)
	{
		setLEDsBatteryPower(energy);
	};
	
void flashLEDs(char sec)
{
	for(char i = 0, i < 10, ++i)
	setLEDsBatteryPower(4);
	_delay_ms(sec * 1000);
	setLEDsBatteryPower(0);
	
};
	
signed char readHallSensor(unsigned char num);			//gets hallvoltage returns wished power (MAX POWER 42.5 A)
	{
		char front = readInterfaceSensorsVoltage(0);	//Hallsensor front
		char tail = readInterfaceSensorsVoltage(1);		//Hallsensor tail
	};
