#ifndef ENERGY_H_INCLUDED
#define ENERGY_H_INCLUDED

enum BatteryState {ok, no_break, low_energy}

void initEnergy();
char getBatteryCharge();
enum BatteryState getBatteryState();

#endif // ENERGY_H_INCLUDED
