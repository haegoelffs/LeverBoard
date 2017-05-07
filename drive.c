/*
last change: 07.5.2017
version: 0.1
*/

#include "drive.h"
#include "system.h"


char give_actualcurrent(char phase_state)
{
	switch(phase_state)
	{
		case 0:
			return readPhaseCurrnet(1);
		case 1:
			return readPhaseCurrnet(2);
		case 2:
			return readPhaseCurrnet(2);
		case 3:
			return readPhaseCurrnet(3);
		case 4:
			return readPhaseCurrnet(3);
		case 5:
			return readPhaseCurrnet(1);
		default:
			return 0;
	}
}



void rise_sink_pwm_dutyc(char new_current,char actual_current, char duty_cycle)
{
	if((actual_current < new_current) && (duty_cycle < 100))
	{
		++duty_cycle;
		setPWMDutyCycle(duty_cycle);
	}
	else if ((actual_current > new_current) && (duty_cycle > 0))
	{
		--duty_cycle;
		setPWMDutyCycle(duty_cycle);
	}
	else duty_cycle= 0;
}
