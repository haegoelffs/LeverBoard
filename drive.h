/*
last change: 08.5.2017
version: 0.1
*/

#ifndef DRIVE_H_INCLUDED
#define DRIVE_H_INCLUDED

#include <stdint.h>



void initDrive();          //Initialices the Zerocrossing. 
void timeroverflow1();     //Is called when the timer has an overflow. Does Nothing
void set_phase_state1();	// Mosfet timing
void set_phase_state2();	// Mosfet timing
void set_phase_state3();	// Mosfet timing
void set_phase_state4();	// Mosfet timing
char give_actualcurrent();  //gives the current of the active phase
char setPWMDutyCycle_dr(char dutyCycle, char current); //Sets the Pwm duty cycle {0,...,100}
void rise_sink_pwm_dutyc(char new_current,char actual_current, char duty_cycle); //Not used at the moment


#endif // DRIVE_H_INCLUDED
