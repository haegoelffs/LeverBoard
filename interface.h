/*
last change: 08.5.2017
version: 0.1
*/

#ifndef INTERFACE_H_INCLUDED
#define INTERFACE_H_INCLUDED

//void initInterface();					//Does nothing at the moment
void setNoBreakAlert();					//Turns piezo on
void quitNoBtreakAlert();				//Turns piezo off

void setBatteryAlert();					//Turns piezo on
void quitBatteryAlert();				//Turns piezo off
void timeroverflow2();					//Called when timer2 has an overflow. Does nothing
void setEnergyLEDs(char energy);		//Sets LEDS(energy = 4 --> 4 LED,   energy = 3 --> 3 LED, energy = 2 --> 2 LED, energy = 1 --> 1 LED, energy = 0 --> 0 LED)
void flashLEDs();						//Flashes all 4 Energy LEDs
signed char give_newcurrent(void);		//Returns new wisched current of the phases, considering the angle of the upper Board (Not in use at the moment)
char give_new_dutycycle();				//Returns the new duty cycle, considering the angle of the board.

#endif // INTERFACE_H_INCLUDED
