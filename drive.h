/*
last change: 08.5.2017
version: 0.1
*/

#ifndef DRIVE_H_INCLUDED
#define DRIVE_H_INCLUDED

#include <stdint.h>

<<<<<<< HEAD
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
void rise_sink_pwm_dutyc(char new_current,char actual_current, char duty_cycle); //Not used at the moment
=======
typedef enum {
            free_running,
            fix_commutated,
            controlled
            } DriveState;

void initDrive(void);

void setPowerLevel(int8_t power);
>>>>>>> origin/master

#endif // DRIVE_H_INCLUDED
