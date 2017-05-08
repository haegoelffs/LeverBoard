/*
last change: 08.5.2017
version: 0.1
*/

#ifndef INTERFACE_H_INCLUDED
#define INTERFACE_H_INCLUDED

void initInterface();


void setNoBreakAlert();
void quitNoBtreakAlert();

void setBatteryAlert(char energy);
void quitBatteryAlert();

void setEnergyLEDs(char energy);     
void flashLEDs(unsigned char tmp, char ms);
signed char give_newcurrent(void);

#endif // INTERFACE_H_INCLUDED
