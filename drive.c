/*
last change: 08.5.2017
version: 0.1
*/
#include "drive.h"
#include "system.h"
#include "global.h"

void initDrive()
{
	 //edge = 0: falling edge
	 // edge = 1: rising edge
	registerVoltageZeroCrossingListenerPhaseA(set_phase_state1,0);
}

void timeroverflow1()
{
	//do nothing
}

void set_phase_state1()
{
	startTimeMeasurement1(timeroverflow1);
	registerVoltageZeroCrossingListenerPhaseA(set_phase_state2,1);
}

void set_phase_state2()
{
	delta_time = stopTimeMeasurement1()/3;
	uint16_t delta_time_half = delta_time/2;
	startAfterMs1(delta_time_half, set_phase_state3);
	registerVoltageZeroCrossingListenerPhaseA(set_phase_state1,0);
}

void set_phase_state3()
{
	changePhaseState(5);
	phaseState = 5;
	startAfterMs1(delta_time, set_phase_state4);
}

void set_phase_state4()
{
	switch (phaseState)
	{
	case 0:
		startAfterMs1(delta_time, set_phase_state4);
		phaseState= 1;
		changePhaseState(1);
		break;
	case 1:
		startAfterMs1(delta_time,  set_phase_state4);
		phaseState= 2;
		changePhaseState(2);
		break;
	case 2:
		startAfterMs1(delta_time, set_phase_state4);
		phaseState= 3;
		changePhaseState(3);
		break;
	case 3:
		startAfterMs1(delta_time, set_phase_state4);
		phaseState= 4;
		changePhaseState(4);
		break;
	}
}

char give_actualcurrent()
{
	switch(phaseState)
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
	setPWMDutyCycle(duty_cycle);
}

