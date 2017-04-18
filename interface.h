#ifndef INTERFACE_H_INCLUDED
#define INTERFACE_H_INCLUDED

void initInterface();


void setNoBreakAlert();
void quitNoBtreakAlert();

void setBatteryAlert(char energy);
void quitBatteryAlert();

void setEnergyLEDs(char energy);     

signed char readHallSensor(int num);

#endif // INTERFACE_H_INCLUDED
