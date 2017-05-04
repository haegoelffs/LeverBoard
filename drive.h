
/*
last change: 04.5.2017
version: 0.1
*/
#ifndef DRIVE_H_INCLUDED
#define DRIVE_H_INCLUDED

#include <stdint.h>

//void initDrive();
char give_actualcurrent(char phase_state);

void rise_sink_pwm_dutyc(char new_current,char actual_current, char duty_cycle);


#endif // DRIVE_H_INCLUDED
