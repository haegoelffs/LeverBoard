/*
last change: 08.5.2017
version: 0.1
*/

#include "system.h"
#include "interface.h"
#include <avr/io.h>
#include <util/delay.h>

static char newData;

void initINTERFACE()
{
    setPowerLED();
}

uint16_t void setNoBreakAlert()
	{
		setPiezoSound(1);
		return getSystimeMs();
	}

void quitNoBtreakAlert(	uint16_t sys)
	{
		uint16_t sys2 = getSystimeMs();
		if ((sys2-sys) > 2000)
		{
			setPiezoSound(0);
		}
	}

 uint16_t void setBatteryAlert(){
		setPiezoSound(1);
		return getSystimeMs();
	}

void quitBatteryAlert(uint16_t sys)
{
	uint16_t sys2 = getSystimeMs();
	if ((sys2-sys) > 1000)
	{
			setPiezoSound(0);
	}
}

void flashLEDs()
{
	for(char i = 0; i < 10; ++i)
	setLEDsBatteryPower(4);
	_delay_ms(1000);
	setLEDsBatteryPower(0);

}

void set_new_dutycycle()
{
	static char duty_cycle;
	char front = getLastHallSensorNoseVoltage();	//Hallsensor front
	char tail =  getLastHallSensorTailVoltage();	   //Hallsensor tail
	char delta_sensors;
	char tobe_current = 0;
	if(front > tail)
	{
		delta_sensors = front - tail;
	}
	else delta_sensors = 0;
	if((delta_sensors < 25)) tobe_current = 0;
	if (delta_sensors > 81) tobe_current = 42;
	else
	{
		tobe_current = (delta_sensors * 42)/81;
	}
	registerMeasurementDataAvailableListener(&new_data_available);
	if (newData)
	{
		char actual_current;
		char phaseState = getPhaseState();
		switch (phaseState)
		{
			case 0: actual_current= getLastPhaseACurrent();
					break;
			case 1: actual_current= getLastPhaseCCurrent();
					break;
			case 2: actual_current= getLastPhaseBCurrent();
					break;
            case 3: actual_current= getLastPhaseACurrent();
					break;
			case 4: actual_current= getLastPhaseCCurrent();
					break;
			case 5: actual_current= getLastPhaseBCurrent();
					break;
		}
		if(((actual_current > tobe_current) && (duty_cycle > 0)) || actual_current > 42)
		{
			--duty_cycle;
		}
		else if ((actual_current < tobe_current && (duty_cycle < 100)) || actual_current < 42)
		{
			++duty_cycle;
		}
		setPWMDutyCycle(duty_cycle);
		newData = 0;
	}

}

void new_data_available(void)
{
	newData = 1;
}

void timeroverflow2()
{
	//do nothing
}
