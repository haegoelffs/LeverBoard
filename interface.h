/*
last change: 08.5.2017
version: 0.1
*/

#ifndef INTERFACE_H_INCLUDED
#define INTERFACE_H_INCLUDED

void initInterface();


void setNoBreakAlert();
void quitNoBtreakAlert();

void setBatteryAlert();
void quitBatteryAlert();
void timeroverflow2();
void setEnergyLEDs(char energy);     
void flashLEDs();
signed char give_newcurrent(void);

#endif // INTERFACE_H_INCLUDED
