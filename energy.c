#include "System/system.h"
#include "energy.h"
#include "interface.h"
#include <avr/io.h>

void emergencyShutDown(char current)
{
	if(current > 90)
	{
		enableBridgeDriver(0);
		setNoBreakAlert();
		while(1);
	}
}







char getBatteryState()
{
	char voltage = getLastBattery();
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

	switch(BatteryState) {
		case 0:
            if(*numLed < 1)
            {
                flashLEDs();
                ++(*numLed) ;
            }
            *numPiezo = 0;
            setLEDsBatteryPower(0);
            setPiezoSound(0);
            break;
		case 1:
            if (*numPiezo < 1)
            {
                *systime = setBatteryAlert();
                ++(*numPiezo);
            }
		else quitBatteryAlert(*systime);
            numLed = 0;
            setLEDsBatteryPower(1);
            break;
		case 2:
            *numLed = 0;
            *numPiezo = 0;
            setLEDsBatteryPower(2);
            setPiezoSound(0);
            break;
		case 3:
            *numLed = 0;
            *numPiezo = 0;
            setLEDsBatteryPower(3);
            setPiezoSound(0);
            break;
		case 4:
            *numLed = 0;
            setLEDsBatteryPower(4);
            if (*numPiezo < 1)
            {
                *systime= setNoBreakAlert();
                ++(*numPiezo);
            }

            else quitNoBtreakAlert(*systime);

	}
}
