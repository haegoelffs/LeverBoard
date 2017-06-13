/*
last change: 08.5.2017
version: 0.1
*/

#ifndef INTERFACE_H_INCLUDED
#define INTERFACE_H_INCLUDED

//Initialices the Interface
void initINTERFACE();

//Turns on piezo
uint16_t setNoBreakAlert();

//Turns piezo off if the time since it is turnd on is bigger/equal  2 sec
void quitNoBtreakAlert(uint16_t sys);

//Turns on piezo
uint16_t setBatteryAlert();

//Turns piezo off if the time since it is turnd on is bigger/equal  1 sec
void quitBatteryAlert(uint16_t sys);

//Called when timer2 has an overflow. Does nothing
void timeroverflow2();

//Flashes all 4 Energy LEDs
void flashLEDs();

//Returns the new duty cycle, considering the angle of the board.
void set_new_dutycycle();

#endif // INTERFACE_H_INCLUDED
