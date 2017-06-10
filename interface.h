/*
last change: 08.5.2017
version: 0.1
*/

#ifndef INTERFACE_H_INCLUDED
#define INTERFACE_H_INCLUDED

void initINTERFACE();                   //Initialices the Interface
uint16_t setNoBreakAlert();					//Turns piezo on

void quitNoBtreakAlert(uint16_t sys);				//Turns piezo off

uint16_t setBatteryAlert();					//Turns piezo on
void quitBatteryAlert(uint16_t sys);				//Turns piezo off

void timeroverflow2();					//Called when timer2 has an overflow. Does nothing
void flashLEDs();						//Flashes all 4 Energy LEDs
void set_new_dutycycle();				//Returns the new duty cycle, considering the angle of the board.
void new_data_available(void);
#endif // INTERFACE_H_INCLUDED
