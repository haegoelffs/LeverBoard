#include "system.h"
#include "leverboardHardware.h"
#include "logger.h"

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

volatile char pwmOut = 0;
volatile unsigned int cnt = 0;

// pwm
void initPWM()
{
    // soft fast pwm
    /*CONTROL_REGISTER_A_PWM_TIMER |= FAST_SOFT_PWM_A;
    CONTROL_REGISTER_B_PWM_TIMER |= FAST_SOFT_PWM_PRESC_1024_B;

    MAX_VALUE_PWM_TIMER = RESOLUTION_PWM;
    COMPARE_VALUE_PWM_TIMER = 50;
    ENABLE_INTERRUPTS_PWM_TIMER |= COMPARE_A_B_INTERRUPT;*/

    // phase correct soft pwm
    CONTROL_REGISTER_A_PWM_TIMER |= PC_SOFT_PWM_A;
    CONTROL_REGISTER_B_PWM_TIMER |= PC_SOFT_PWM_PRESC_8_B;

    MAX_VALUE_PWM_TIMER = RESOLUTION_PWM;
    COMPARE_VALUE_PWM_TIMER = 50;
    ENABLE_INTERRUPTS_PWM_TIMER |= COMPARE_B_INTERRUPT;
}

ISR (TIMER0_COMPB_vect)
{
    cnt++;

    //logVar("cnt",cnt,2);

    if(cnt>5000)
    {
        if(pwmOut)
        {
            // set output on low
            setLEDsBatteryPower(0);
            pwmOut = 0;
        }
        else
        {
            // set output on high
            setLEDsBatteryPower(4);
            pwmOut = 1;
        }
        cnt = 0;
    }
}

ISR (TIMER0_COMPA_vect)
{

}


void changePhaseState(char phase, enum phaseState state)
{
		
}

/*
dutyCycle = 0 - 100, alles >100 entspricht 100
*/
void setPWMDutyCycle(char dutyCycle)
{
    if(dutyCycle > 100)
    {
        dutyCycle = 100;
    }

    char newCompareValue = dutyCycle*RESOLUTION_PWM/100;
    COMPARE_VALUE_PWM_TIMER = newCompareValue;
}

// timers
void startAfterMs(uint16_t ms, void (*fn)(void))
{
    (*fn)();
}

// analog values

// gpios
void initGPIOs()
{
    // outputs
    TRISTATE_LEDS |= (1<<PIN_LED0);
    TRISTATE_LEDS |= (1<<PIN_LED1);
    TRISTATE_LEDS |= (1<<PIN_LED2);
    TRISTATE_LEDS |= (1<<PIN_LED3);

    TRISTATE_ENABLE_DRIVER |= (1<<PIN_ENABLE_DRIVER);

    TRISTATE_PIEZO |= (1<<PIN_PIEZO);
}

void setLEDsBatteryPower(char batteryPower)
{
    if(batteryPower>3)
    {
        PORT_LEDS |= (1<<PIN_LED3);
    }
    else
    {
        PORT_LEDS &= ~(1<<PIN_LED3);
    }

    if(batteryPower>2)
    {
        PORT_LEDS |= (1<<PIN_LED2);
    }
    else
    {
        PORT_LEDS &= ~(1<<PIN_LED2);
    }

    if(batteryPower>1)
    {
        PORT_LEDS |= (1<<PIN_LED1);
    }
    else
    {
        PORT_LEDS &= ~(1<<PIN_LED1);
    }

    if(batteryPower>0)
    {
        PORT_LEDS |= (1<<PIN_LED0);
    }
    else
    {
        PORT_LEDS &= ~(1<<PIN_LED0);
    }
}

void enableBridgeDriver(char enable)
{
    if(enable)
    {
        PORT_ENABLE_DRIVER |= (1<<PIN_ENABLE_DRIVER);
    }
    else
    {
        PORT_ENABLE_DRIVER &= ~(1<<PIN_ENABLE_DRIVER);
    }
}

void setPiezoSound(char state)
{
    if(state)
    {
        PORT_PIEZO |= (1<<PIN_PIEZO);
    }
    else
    {
        PORT_PIEZO &= ~(1<<PIN_PIEZO);
    }
}

// uart
void initUART()
{
    UART_REGISTER_A |= UART_REGISTER_A_VALUE;
    UART_REGISTER_B |= UART_REGISTER_B_VALUE;
    UART_REGISTER_C |= UART_REGISTER_C_VALUE;

    UART_BAUD_RATE_HIGH_BYTE = UART_BAUD_RATE_HIGH_BYTE_VALUE;
    UART_BAUD_RATE_LOW_BYTE = UART_BAUD_RATE_LOW_BYTE_VALUE;
}

// Source: https://www.mikrocontroller.net/articles/AVR-GCC-Tutorial/Der_UART
void transmitString(char *msg)
{
    while (*msg) // point on first char
    {
        // msg is zero terminated -> loop will stop at the end of the string
        transmitChar(*msg);
        msg++; // increase pointer to next char
    }
}

// Source: Atmel ATmega640/V-1280/V-1281/V-2560/V-2561/V Datasheet p.207
void transmitChar(char data)
{
    while(!(UART_REGISTER_A & (1<<UART_DATA_REG_EMPTY_BIT)));

    UART_SEND_DATA = data;
}
