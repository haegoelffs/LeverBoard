/*
 */
#include <avr/io.h>
#include <stdint.h>

#include "drive.h"
#include "system.h"
#include "interface.h"
#include "energy.h"

int main(void)
{
    // init Hardware
    initUART();
    initGPIOs();
    initAnalog();
    initPWM();

    // init modules
    initEnergy();
    initInterface();
    initDrive();

    // variables
    char BatteryState batteryState = BatteryState.ok;

    while(1)
    {
        // check break state
        batteryState = getBatteryState();
        if(batteryState==BatteryState.no_break)
        {
            setNoBreakAlert();
        }
        else
        {
            quitNoBtreakAlert();
        }

        // handle battery charge
        setEnergyLEDs(getBatteryCharge());

        // handle acceleration / deceleration
        signed char boardPosition = getBoardPosition();

        if(boardPosition<0)
        {
            // break
            if(batteryState!=BatteryState.no_break)
            {
                setRotationsPerSec(0);
                setBreakCoefficient((char)(boardPosition*(-1)));
            }
        }
        else
        {
            setRotationsPerSec();
        }

    }

    return 0;
}
