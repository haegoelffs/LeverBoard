#include system.h
#include <avr/io.h>

void initInterface()
		{
		};

void setNoBreakAlert()
	{
		setPiezoSound(1);   
	};
	
void quitNoBtreakAlert()
	{
		setPiezoSound(1);   
	};

void setBatteryAlert()									//energy is the Battery voltage, it is a value from 1 to 4 (1 is very low, 4 is very good)
	{
		setPiezoSound(1);     
	};
	
void quitBatteryAlert()
	{
		setPiezoSound(0);
	};
	
void setEnergyLEDs(unsigned char energy);				//energy is the Battery voltage, it is a value from 1 to 4 (1 is very low, 4 is very good)
	{
		setLEDsBatteryPower(energy);
	};
	
signed char readHallSensor(unsigned char num);			//gets hallvoltage returns wished power (MAX POWER 42.5 A)
	{
		char front = readInterfaceSensorsVoltage(0);	//Hallsensor front
		char tail = readInterfaceSensorsVoltage(1);		//Hallsensor tail
	};
