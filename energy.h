
/*
last change: 08.5.2017
version: 0.1
*/

#ifndef ENERGY_H_INCLUDED
#define ENERGY_H_INCLUDED

//void initEnergy();
char getBatteryState();  //returns a char from from 0 to 4 (1 is very low voltage (0 turns off, 1 piezo, 4 no charging)
void emergencyShutDown(char current);    //Shuts the Pwm off if current > 47 Ampére and turns on piezo

#endif // ENERGY_H_INCLUDED