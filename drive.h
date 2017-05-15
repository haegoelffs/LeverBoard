/*
last change: 08.5.2017
version: 0.1
*/

#ifndef DRIVE_H_INCLUDED
#define DRIVE_H_INCLUDED

#include <stdint.h>

void initDrive();          //Initialices the Zerocrossing. 
void set_time1(char edge);	// Mosfet timing
void set_time2(char edge);	// Mosfet timing
void set_phase_state_s();	// Mosfet timing
void set_phase_state();     // Mosfet timing
void timeroverflow1();     //Is called when the timer has an overflow. Does Nothing
void disableCompA();
void enableCompA();
char give_actualcurrent(char phase);  //gives the current of the active phase
char setPWMDutyCycle_dr(char dutyCycle, char current); //Sets the Pwm duty cycle {0,...,100}


#endif // DRIVE_H_INCLUDED
