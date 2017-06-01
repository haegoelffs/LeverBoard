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
	setEnableCompA(1);
	registerVoltageZeroCrossingListenerPhaseA(&set_time1);
}

void set_time1(char edge)
{
    if(edge == 0)
    {
		disableCompA();
        startTimeMeasurement(&timeroverflow1);
        registerVoltageZeroCrossingListenerPhaseA(&set_time2);
    }
}

void set_time2(char edge)
{
    if(edge == 1)
    {
		disableCompA();
        delta_time = stopTimeMeasurement()/3;
        uint16_t delta_time_half = delta_time/2;
		startAfterUs(delta_time_half, &set_phase_state_s);
        registerVoltageZeroCrossingListenerPhaseA(&set_time1);
    }
}

void set_phase_state_s()
{
	phaseState = 5;
	changePhaseState(5);
	startAfterUs(delta_time, &set_phase_state);
}

void set_phase_state()
{
	switch (phaseState)
	{
		case 0:
			enableCompA();
			startAfterUs(delta_time, &set_phase_state);
			phaseState= 1;
			changePhaseState(1);
			break;
		case 1:
			startAfterUs(delta_time, &set_phase_state);
			phaseState= 2;
			changePhaseState(2);
			break;
		case 2:
			startAfterUs(delta_time, &set_phase_state);
			phaseState= 3;
			changePhaseState(3);
			break;
		case 3:
			enableCompA();
			startAfterUs(delta_time,  &set_phase_state);
			phaseState= 4;
			changePhaseState(4);
			break;
		case 4:
			// Do nothing
			break;
		case 5:
			startAfterUs(delta_time, &set_phase_state);
			phaseState= 6;
			changePhaseState(6);
			break;
	}
}

void timeroverflow1()
{
	//logVar("over",4,10);
}

void disableCompA()
{
	if(enable == 0)
	{
		setEnableCompA(0);
	}
}

void enableCompA()
{
	if(enable == 0)
	{
		setEnableCompA(1);
	}
}

char give_actualcurrent(char phase)
{
	
	switch(phase)
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

char setPWMDutyCycle_dr(char dutyCycle, char current)
{
	if(current <= 42)
	{
		setPWMDutyCycle(dutyCycle);
		return dutyCycle;
	}
	else if (dutyCycle != 0) 
	{
		while((give_actualcurrent(phaseState) > 42) && (dutyCycle > 0))
		{
			dutyCycle -= 5;
			setPWMDutyCycle(dutyCycle);
			
		}
		
		return (dutyCycle);
	}
	return 0;
}


/*
void timeroverflow1()
{
	logVar("over",4,10);
}

void initDrive()
{
	//edge = 0: falling edge
	// edge = 1: rising edge
	setEnableCompB(1);
	registerVoltageZeroCrossingListenerPhaseB(&set_phase_state1);
	setEnableCompB(1);
}

void set_phase_state1(char edge)
{
	
	if(edge == 1)
	{
		//setEnableCompB(0);
		startTimeMeasurement(&timeroverflow1);
		//setEnableCompA(1);
		registerVoltageZeroCrossingListenerPhaseA(&set_phase_state2);
	}
	
	//logVar("edge",edge,10);
}

void set_phase_state2(char edge)
{
	if(edge == 0)
	{
		//setEnableCompA(0);
		delta_time = stopTimeMeasurement();
		uint16_t delta_time_half = delta_time/2;
		startAfterUs(delta_time_half, set_phase_state3);
		//setEnableCompB(1);
		registerVoltageZeroCrossingListenerPhaseB(&set_phase_state1);
	}
}

void set_phase_state3()
{
	phaseState = 2;
	changePhaseState(2);
	startAfterUs(delta_time, &set_phase_state4);
}

void set_phase_state4()
{
	switch (phaseState)
	{
		case 2:
			startAfterUs(delta_time, &set_phase_state4);
			phaseState= 3;
			changePhaseState(3);
			break;
		case 3:
			startAfterUs(delta_time, &set_phase_state4);
			phaseState= 4;
			changePhaseState(4);
			break;
		case 4:
			startAfterUs(delta_time, &set_phase_state4);
			phaseState= 5;
			changePhaseState(5);
			break;
		case 5:
			startAfterUs(delta_time, &set_phase_state4);
			phaseState= 0;
			changePhaseState(0);
			break;
		case 0:
			startAfterUs(delta_time, &set_phase_state4);
			phaseState= 1;
			changePhaseState(1);
			break;
		
		
	}
}
*/