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
void changePhaseState(char phase_state);

// timers
void startAfterMs(uint16_t ms, void (*fn)(void));

// analog values
void initAnalog();

//void registerVoltageZeroCrossingListener(function callback);
void setVoltageZeroCrossingPhase(char phase);

// value * 10⁻1 -> 232 = 23,2 V
//void startIntegration(char limit, function callback);

char readBatteryVoltage();
char readPhaseCurrnet(char phase);
char readPhaseVoltage(char voltage);

// sensor 0 = Front
// sensor 1 = Back
char readInterfaceSensorsVoltage(char sensor);

// gpios
void initGPIOs();
void setLEDsBatteryPower(char batteryPower);    // batteryPower = 0 --> all leds off, = 1 --> led1 on, = 2 --> led2 on ...
void enableBridgeDriver(char enable);
void setPiezoSound(char state);                 //(state == 1) ==> piezo on,  (state == 0) ==> piezo off
void switchPwmOnOff(char state);             //Turn PWM on (state == 1) and Turn PWM off(state == 0)

// log
void initUART();
void transmitString(char *msg);
void transmitChar(char data);

#endif // SYSTEM_H_INCLUDED
