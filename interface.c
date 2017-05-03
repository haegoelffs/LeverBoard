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
		//Start Timer_user 
	};
	
void quitNoBtreakAlert()
	{
		/*If Timer Timer_user1 >= 1 sek
		{
			//setPiezoSound(0);
			//stop Timer_user1 + reset Timer_user1;
		}
		*/
	};

void setBatteryAlert()									//energy is the Battery voltage, it is a value from 1 to 4 (1 is very low, 4 is very good)
	{
		
		setPiezoSound(1);
		//Start Timer_user1
		
	};
	
void quitBatteryAlert()
	{
		/*If Timer Timer_user1 >= 1 sek
		{
			//setPiezoSound(0);
			//stoptimer + reset Timer;
		}
		*/  
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
	
signed char give_newcurrent(void);			//gets hallvoltage returns wished power (MAX POWER 42.5 A)
	{
		char front = readInterfaceSensorsVoltage(0);	//Hallsensor front
		//char tail = readInterfaceSensorsVoltage(1);		//Hallsensor tail									
		if(front < (char)10)							//correction factor for hall sensors (ANPASSEN!!!)	
		{
			char front = 0;
		}									
		return (signed char) (front * (char) 40) / (char) 255;     //Return wished current(40 set as Maximum)
	};
