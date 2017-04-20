/*
last change: 18.4.2017
version: 0.1
*/

#ifndef SYSTEM_H_INCLUDED
#define SYSTEM_H_INCLUDED

#include <stdint.h>

// pwm
enum phaseState {on, off};

void initPWM();
void setPWMDutyCycle(char dutyCycle);
void changePhaseState(char phase, enum phaseState state);

// timers
void startAfterMs(uint16_t ms, function callback);

// analog values
void initAnalog();

void registerVoltageZeroCrossingListener(function callback);
void setVoltageZeroCrossingPhase(char phase);

// value * 10⁻1 -> 232 = 23,2 V
void startIntegration(char limit, function callback);

char readBatteryVoltage();
char readPhaseCurrnet(char phase);
char readPhaseVoltage(char voltage);

// sensor 0 = Front
// sensor 1 = Back
char readInterfaceSensorsVoltage(char sensor);

// gpios
void initGPIOs();
void setLEDsBatteryPower(char batteryPower);
void enableBridgeDriver(char enable);
void setPiezoSound(char state);                 //(state == 1) ==> piezo on,  (state == 0) ==> piezo off

// log
void initUART();
void logMsg(char *msg);


#endif // SYSTEM_H_INCLUDED
