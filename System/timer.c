// timer
#include "system.h"
#include "logger.h"
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

/** Calculations

Start after callback:
    Prescaler: 1/64
        1/(16e6/64) = 4us
        - Timermax
        4us * 2¹⁶ = 262.2ms

Systime:
    Prescaler: 1/8
        1/(16e6/8) = 500ns
        - Zeitbereich 16bit:
        500ns * 2¹⁶ = 32.8ms
        - Zeitbereich 24bit:
        500ns * 2²⁴ = 8.4s
        - Zeitbereich 32bit
        500ns * 2³² = 2147.5s = 35.8min
    Prescaler: 1/64
        1/(16e6/64) = 4us
        - Zeitbereich 16bit:
        4us * 2¹⁶ = 262.2ms
        - Zeitbereich 24bit:
        4us * 2²⁴ = 67s
        - Zeitbereich 32bit
        4us * 2³² = 17179.9s = 286.3min = 4.8h
*/


/** TCCR1A – Timer/Counter 1 Control Register A p.154
    |7      |6      |5      |4      |3      |2      |1      |0      |
    |COM1A1 |COM1A0 |COM1B1 |COM1B0 |COM1C1 |COM1C0 |WGM11  |WGM10  |

Bit 7:6 – COMnA1:0: Compare Output Mode for Channel A
Bit 5:4 – COMnB1:0: Compare Output Mode for Channel B
Bit 3:2 – COMnC1:0: Compare Output Mode for Channel C
kontrolliert Verhalten der output compare pins (OCnA, OCnB, OCnC):

COMnA0 = 0 & COMnA1 = 0:
Normal port operation, OCnA/OCnB/OCnC disconnected
Bit 1:0 – WGMn1:0: Waveform Generation Mode
kontrolliert mit bits WGMn3:2 in TCCRnB den Waveform Generation Mode (Fast PWM, CTC etc.)

Normal Mode:
Timer zählt hoch bis 0xFFFF, danach wird Overflow Interrupt ausgelöst und Zähler zurückgesetzt.
WGM3:0 = 0000
*/

// start after
#define START_AFTER_TIMER_CONTROLL_REGISTER_A TCCR1A
#define START_AFTER_TIMER_CONTROLL_REGISTER_A_value ((0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<COM1C1) | (0<<COM1C0) | (0<<WGM11) | (0<<WGM10))

// systime
#define SYSTIME_TIMER_CONTROLL_REGISTER_A TCCR3A
#define SYSTIME_TIMER_CONTROLL_REGISTER_A_value ((0<<COM3A1) | (0<<COM3A0) | (0<<COM3B1) | (0<<COM3B0) | (0<<COM3C1) | (0<<COM3C0) | (0<<WGM31) | (0<<WGM30))

/** TCCR1B – Timer/Counter 1 Control Register B p.156
    |7      |6      |5      |4      |3      |2      |1      |0      |
    |ICNC1  |ICES1  |-      |WGM13  |WGM12  |CS12   |CS11   |CS10   |

Bit 7 – ICNCn: Input Capture Noise Canceler
Falls über Pin ICPn Counter erhöht wird, kann mittels NoiseCanceler der Input gefiltert werden.
ICNCn = 1: activatet
ICNCn = 0: deactivated

Bit 6 – ICESn: Input Capture Edge Select
Falls über Pin ICPn Counter erhöht wird, kann festgelegt werden ob bei steigender oder fallender Flanke

Bit 4:3 – WGMn3:2: Waveform Generation Mode
Mit Bit 1:0 aus TCCR1A.

Bit 2:0 – CSn2:0: Clock Select
Table 17-6 p.157
CSn0 = 0, CSn1 = 0, CSn2 = 0: No clock source (stopped)
CSn0 = 0, CSn1 = 0, CSn2 = 1: clk/1
CSn0 = 0, CSn1 = 1, CSn2 = 0: clk/8
CSn0 = 0, CSn1 = 1, CSn2 = 1: clk/64
...
CSn0 = 1, CSn1 = 0, CSn2 = 1: clk/1024
...
*/
#define START_AFTER_TIMER_CONTROLL_REGISTER_B TCCR1B
#define START_AFTER_TIMER_CONTROLL_REGISTER_B_value ((0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (0<<CS12) | (0<<CS11) | (0<<CS10))

#define START_AFTER_TIME_START (START_AFTER_TIMER_CONTROLL_REGISTER_B |= ((0<<CS12) | (1<<CS11) | (1<<CS10)))
#define START_AFTER_TIME_STOP (START_AFTER_TIMER_CONTROLL_REGISTER_B &= ~((1<<CS12) | (1<<CS11) | (1<<CS10)))

#define SYSTIME_TIMER_CONTROLL_REGISTER_B TCCR3B
#define SYSTIME_TIMER_CONTROLL_REGISTER_B_value ((0<<ICNC3) | (0<<ICES3) | (0<<WGM33) | (0<<WGM32) | (0<<CS32) | (0<<CS31) | (0<<CS30))

#define SYSTIME_START (SYSTIME_TIMER_CONTROLL_REGISTER_B |= ((0<<CS32) | (1<<CS31) | (1<<CS30)))
#define SYSTIME_STOP (SYSTIME_TIMER_CONTROLL_REGISTER_B &= ~((1<<CS32) | (1<<CS31) | (1<<CS30)))

/** TCCR1C – Timer/Counter 1 Control Register C
    |7      |6      |5      |4      |3      |2      |1      |0      |
    |FOC1A  |FOC1B  |FOC1C  |-      |-      |-      |-      |-      |

Bit 7 – FOCnA: Force Output Compare for Channel A
Bit 6 – FOCnB: Force Output Compare for Channel B
Bit 5 – FOCnC: Force Output Compare for Channel C
Nur aktiv falls ein no-PWM-Mode gewählt ist. Wenn auf 1 gestezt wird, wird compare match
auf waveform generation unit gegeben. Löst jedoch keinen Interrupt aus oder setzt Timer auf 0 bei CTC.
*/

/** TCNT1H and TCNT1L – Timer/Counter 1 p.158
|7      |6      |5      |4      |3      |2      |1      |0      |
|TCNT15 |TCNT14 |TCNT13 |TCNT12 |TCNT11 |TCNT10 |TCNT9  |TCNT8  |  TCNT1H

|7      |6      |5      |4      |3      |2      |1      |0      |
|TCNT7  |TCNT6  |TCNT5  |TCNT4  |TCNT3  |TCNT2  |TCNT1  |TCNT0  |  TCNT1L

Wert timer 1.

Muss speziel angesprochen werden:
Accessing 16-bit Registers p.135
Wenn 16bit Register gelesen wird, muss zuerst das Low-Byte gelsesen werden (Das High-Byte wird von diversen Registern geteilt)
Wenn 16bit beschrieben werden soll, muss zuerst das High-Byte beschrieben werden.
*/
#define START_AFTER_TIMER_VALUE_HIGH TCNT1H
#define START_AFTER_TIMER_VALUE_LOW TCNT1L

#define SYSTIME_TIMER_VALUE_HIGH TCNT3H
#define SYSTIME_TIMER_VALUE_LOW TCNT3L

/** OCR1AH and OCR1AL – Output Compare Register 1 A p.159
|7      |6      |5      |4      |3      |2      |1      |0      |
|OCR1A15|OCR1A14|OCR1A13|OCR1A12|OCR1A11|OCR1A10|OCR1A9 |OCR1A8 |  OCR1AH

|7      |6      |5      |4      |3      |2      |1      |0      |
|OCR1A7 |OCR1A6 |OCR1A5 |OCR1A4 |OCR1A3 |OCR1A2 |OCR1A1 |OCR1A0 |  OCR1AL

Vergleichswert Timer 1 Kanal A.
*/
#define START_AFTER_TIMER_COMPARE_VALUE_HIGH OCR1AH
#define START_AFTER_TIMER_COMPARE_VALUE_LOW OCR1AL

/** ICR1H and ICR1L – Input Capture Register 1 p.161
|7      |6      |5      |4      |3      |2      |1      |0      |
|ICR115 |ICR114 |ICR113 |ICR112 |ICR111 |ICR110 |ICR109 |ICR108 |  ICR1H

|7      |6      |5      |4      |3      |2      |1      |0      |
|ICR107 |ICR106 |ICR105 |ICR104 |ICR103 |ICR102 |ICR101 |ICR100 |  ICR1L

Wird mit Counter TCNTn erhöht, wenn event an ICPn Pin anliegt.
*/

/** TIMSK1 – Timer/Counter 1 Interrupt Mask Register
|7      |6      |5      |4      |3      |2      |1      |0      |
|-      |-      |ICIE1  |-      |OCIE1C |OCIE1B |OCIE1A |TOIE1  |

Bit 5 – ICIEn: Timer/Countern, Input Capture Interrupt Enable
Bit 3 – OCIEnC: Timer/Countern, Output Compare C Match Interrupt Enable
Bit 2 – OCIEnB: Timer/Countern, Output Compare B Match Interrupt Enable
Bit 1 – OCIEnA: Timer/Countern, Output Compare A Match Interrupt Enable
Bit 0 – TOIEn: Timer/Countern, Overflow Interrupt Enable
*/
#define START_AFTER_TIMER_INTERRUPT_MASK TIMSK1
#define START_AFTER_TIMER_INTERRUPT_MASK_value ((0<<ICIE1) | (0<<OCIE1C) | (0<<OCIE1B) | (1<<OCIE1A) | (0<<TOIE1))

#define SYSTIME_TIMER_INTERRUPT_MASK TIMSK3
#define SYSTIME_TIMER_INTERRUPT_MASK_value ((0<<ICIE3) | (0<<OCIE3C) | (0<<OCIE3B) | (0<<OCIE3A) | (1<<TOIE3))

/** TIFR1 – Timer/Counter1 Interrupt Flag Register
|7      |6      |5      |4      |3      |2      |1      |0      |
|-      |-      |ICF1   |-      |OCF1C  |OCF1B  |OCF1A  |TOV1   |

Flags des entsprechenden Interrupts.
*/

/** Interrupt Vectors
*/
#define START_AFTER_TIMER_COMPARE_ISR TIMER1_COMPA_vect
#define SYSTIME_OVERFLOW_ISR TIMER3_OVF_vect

volatile uint8_t measurementRunning = 0;

void (*startAfterCallback)(void);
void (*systimeOverflowCallback)(void);

void initTimers()
{
    logMsg("Init Timers...");

    START_AFTER_TIMER_CONTROLL_REGISTER_A |= START_AFTER_TIMER_CONTROLL_REGISTER_A_value;
    START_AFTER_TIMER_CONTROLL_REGISTER_B |= START_AFTER_TIMER_CONTROLL_REGISTER_B_value;

    START_AFTER_TIMER_INTERRUPT_MASK |= START_AFTER_TIMER_INTERRUPT_MASK_value;

    startAfterCallback = 0;

    SYSTIME_TIMER_CONTROLL_REGISTER_A |= SYSTIME_TIMER_CONTROLL_REGISTER_A_value;
    SYSTIME_TIMER_CONTROLL_REGISTER_B |= SYSTIME_TIMER_CONTROLL_REGISTER_B_value;

    SYSTIME_TIMER_INTERRUPT_MASK |= SYSTIME_TIMER_INTERRUPT_MASK_value;
}

// interrupts
ISR (START_AFTER_TIMER_COMPARE_ISR)
{
    START_AFTER_TIME_STOP;
    START_AFTER_TIMER_COMPARE_VALUE_HIGH = 0;
    START_AFTER_TIMER_COMPARE_VALUE_LOW = 0;

    START_AFTER_TIMER_VALUE_HIGH = 0;
    START_AFTER_TIMER_VALUE_LOW = 0;

    void (*temp)(void);
    temp = startAfterCallback;
    startAfterCallback = 0;

    temp();
}

ISR (SYSTIME_OVERFLOW_ISR)
{
    if(systimeOverflowCallback!=0)
    {
        systimeOverflowCallback();
    }
}

/** Calls the handed function after the handed time
Parameter:
time_ us    = time in us. max value: 2¹⁶ * 4 = 262'144us.
fn          = callback. Called after the handed time.
*/
void startAfterUs(uint32_t time_us, void (*fn)(void))
{
    if(startAfterCallback == 0)
    {
        if(time_us > 262144)
        {
            time_us = 262144;
        }

        // register new
        startAfterCallback = fn;

        uint16_t compare = time_us/4;
        START_AFTER_TIMER_COMPARE_VALUE_HIGH = (char)(compare>>8);
        START_AFTER_TIMER_COMPARE_VALUE_LOW = (char)compare;

        START_AFTER_TIME_START;
    }
}

void startTimeMeasurement(void (*timerOverflowCallback)(void))
{
    if(!measurementRunning)
    {
        SYSTIME_START;

        systimeOverflowCallback = timerOverflowCallback;
        measurementRunning = 1;
    }
}

uint8_t isTimeMeasurementRunning()
{
    return measurementRunning;
}

/**
 return: measured time in us. Max. value = 65.536ms
*/
uint16_t stopTimeMeasurement()
{
    SYSTIME_STOP;

    measurementRunning = 0;

    uint8_t low = SYSTIME_TIMER_VALUE_LOW;
    uint8_t high = SYSTIME_TIMER_VALUE_HIGH;

    uint16_t temp = ( (((uint16_t)high)<<8) + low );

    SYSTIME_TIMER_VALUE_HIGH = 0;
    SYSTIME_TIMER_VALUE_LOW = 0;
    return (temp*4);
}
