#ifndef ENERGY_H_INCLUDED
#define ENERGY_H_INCLUDED


//returns a char from from 0 to 4 (0 is a very low voltage and
//4 is a very High(0 turns off, 1 piezo, 4 no charging)
char getBatteryState();



//Shuts the Pwm off if current > 47 Ampére and turns on piezo
void emergencyShutDown(char current);

//Turns on/off Battery LEDs Piezo and the bridge driver considering the Battery_state
//(BatteryState == 0) => Flash LEDs and Turn off bridge driver and BatteryLEDs
//(BatteryState == 1) => Make Piezo sound and turn on one battery led
//(BatteryState == 2) => turn on two battery led
//(BatteryState == 3) => turn on three battery led
//(BatteryState == 4) => turn on four battery led and make piezo sound
void handle_batteryState(char* numLed, char* numPiezo, uint16_t* systime);

#endif // ENERGY_H_INCLUDED
