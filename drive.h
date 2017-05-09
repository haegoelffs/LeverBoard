/*
last change: 08.5.2017
version: 0.1
*/

#ifndef DRIVE_H_INCLUDED
#define DRIVE_H_INCLUDED

#include <stdint.h>



void initDrive();
void timeroverflow1();
void set_phase_state1();
void set_phase_state2();
void set_phase_state3();
void set_phase_state4();
char give_actualcurrent();

void rise_sink_pwm_dutyc(char new_current,char actual_current, char duty_cycle);


#endif // DRIVE_H_INCLUDED
