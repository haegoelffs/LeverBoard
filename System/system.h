#ifndef SYSTEM_H_INCLUDED
#define SYSTEM_H_INCLUDED

#include <stdint.h>

// pwm
void initPWM();

/** Sets the dutycycle of the pwm.
    Input:
    0 <= dutyCycle <= 100 every handed value > 100 will be interpreted as 100
 **/
void setPWMDutyCycle(uint8_t dutyCycle);

/** Changes the output channels for the pwm.
    Input:
    state = 0: A heavyside, C lowside
    state = 1: B heavyside, C lowside
    state = 2: B heavyside, A lowside
    state = 3: C heavyside, A lowside
    state = 4: C heavyside, B lowside
    state = 5: A heavyside, B lowside
    state > 5: Will be ignored
**/
void changePhaseState(uint8_t state);

// timers
void initTimers();

/** Calls the handed function after the handed time
Parameter:
time_ us    = time in us. max value: 2¹⁶ * 4 = 262'144us.
fn          = callback. Called after the handed time.
*/
void startAfterUs(uint32_t time_us, void (*fn)(void));

void startTimeMeasurement(void (*timerOverflowCallback)(void));
uint8_t isTimeMeasurementRunning();
uint16_t stopTimeMeasurement();

// analog values
void initAnalog();
char readBatteryVoltage();
char readPhaseCurrnet(char phase); //value between 0 and 42, where 42 stands for 42 Ampére

// sensor 0 = Front
// sensor 1 = Back
char readInterfaceSensorsVoltage(char sensor);

char readReference1();
char readReference2();
char readReference3();
char readReference4();

// comperators
void initComp();

/** Register the handed function as listener which is called when the voltage of the phase A crosses the zero
    Input:
    listener = function with parameter edge.
        --> edge = 0: falling edge
        --> edge = 1: rising edge
**/
void registerVoltageZeroCrossingListenerPhaseA(void (*listener)(char));
void registerVoltageZeroCrossingListenerPhaseB(void (*listener)(char));
void registerVoltageZeroCrossingListenerPhaseC(void (*listener)(char));

/** Enables the comperator for the phase A.
    Input:
    enable = 1: enable comperator
    enable = 0: disable comperator
**/
void setEnableCompA(char enable);
void setEnableCompB(char enable);
void setEnableCompC(char enable);

// gpios
void initGPIOs();
void setLEDsBatteryPower(char batteryPower);    // batteryPower = 0 --> all leds off, = 1 --> led1 on, = 2 --> led2 on ...
void setPiezoSound(char state);                 //(state == 1) ==> piezo on,  (state == 0) ==> piezo off
void switchPwmOnOff(char state);             //Turn PWM on (state == 1) and Turn PWM off(state == 0)
void setPowerLED();                          // Turns on Power LED

// log
void initUART();
void transmitString(char *msg);
void transmitChar(char data);

#endif // SYSTEM_H_INCLUDED
