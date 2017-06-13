#include "System/system.h"
#include "interface.h"
#include "drive.h"
#include "System/logger.h"
#include <avr/io.h>
#include <util/delay.h>

#define TOLERANCE_HALL_SENS_ZERO_POSITION 25 // 0.5V
#define MAX_HALL_SENS 30     //Max hall sensor voltage as a char

#define MAX_PWR_LEVEL 60     //Max power level as a char

//static char newData;

void initINTERFACE()
{
    setPowerLED();
}


uint16_t setNoBreakAlert()
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

 uint16_t setBatteryAlert()
 {
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
    char i;
	for(i = 0; i < 10; ++i)
	setLEDsBatteryPower(4);
	_delay_ms(1000);
	setLEDsBatteryPower(0);

}

void set_new_dutycycle()
{
	char front = getLastHallSensorNoseVoltage();	//Hallsensor front
	char tail =  getLastHallSensorTailVoltage();	   //Hallsensor tail
	char delta_sensors;
	char tobe_current = 0;
	if(front > tail)
	{
		delta_sensors = front - tail;
	}
	else delta_sensors = 0;
	if((delta_sensors < TOLERANCE_HALL_SENS_ZERO_POSITION)) tobe_current = 0;
	if (delta_sensors > MAX_HALL_SENS) tobe_current = MAX_PWR_LEVEL;
	else
	{
		tobe_current = (delta_sensors * MAX_PWR_LEVEL)/MAX_HALL_SENS;
	}

	setPowerLevel(tobe_current);
}

void timeroverflow2()
{
	//do nothing
}
