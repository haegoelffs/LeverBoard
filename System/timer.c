// timer
#include "system.h"
#include "logger.h"
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>


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

CTC:
TOP = OCRnA
WGM3:0 = 0100

Normal:
TOP = 0xFFFF
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
...
CSn0 = 1, CSn1 = 0, CSn2 = 1: clk/1024
...
*/
#define START_AFTER_TIMER_CONTROLL_REGISTER_B TCCR1B
#define START_AFTER_TIMER_CONTROLL_REGISTER_B_value ((0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (0<<CS12) | (0<<CS11) | (0<<CS10))

#define START_AFTER_TIME_START (START_AFTER_TIMER_CONTROLL_REGISTER_B |= ((1<<CS12) | (0<<CS11) | (1<<CS10)))
#define START_AFTER_TIME_STOP (START_AFTER_TIMER_CONTROLL_REGISTER_B &= ~((1<<CS12) | (1<<CS11) | (1<<CS10)))

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


/** TIFR1 – Timer/Counter1 Interrupt Flag Register
|7      |6      |5      |4      |3      |2      |1      |0      |
|-      |-      |ICF1   |-      |OCF1C  |OCF1B  |OCF1A  |TOV1   |

Flags des entsprechenden Interrupts.
*/

void (*startAfterCallback)(void);

void initTimers()
{
    START_AFTER_TIMER_CONTROLL_REGISTER_A |= START_AFTER_TIMER_CONTROLL_REGISTER_A_value;
    START_AFTER_TIMER_CONTROLL_REGISTER_B |= START_AFTER_TIMER_CONTROLL_REGISTER_B_value;

    START_AFTER_TIMER_INTERRUPT_MASK |= START_AFTER_TIMER_INTERRUPT_MASK_value;

    startAfterCallback = 0;
}

// interrupts
ISR (TIMER1_COMPA_vect)
{
    //logMsg("TIMER INTERRUPT!");

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

/** Calls the handed function after the handed time
**/
void startAfterMs1(uint16_t ms, void (*fn)(void))
{
    // f = 16MHz / 1024 = 15.625kHz
    // T = 1/f = 64us
    // compatre = ms*15.6;

    if(startAfterCallback == 0)
    {
        // register new
        startAfterCallback = fn;

        uint16_t compare = ms*16;
        START_AFTER_TIMER_COMPARE_VALUE_HIGH = (char)(compare>>8);
        START_AFTER_TIMER_COMPARE_VALUE_LOW = (char)compare;

        START_AFTER_TIME_START;

        //logMsg("start after ms timer started");
    }
}

void startTimeMeasurement1(void (*timerOverflowCallback)(void))
{
}
uint16_t getTime1()
{
    return 0;
}
uint16_t stopTimeMeasurement1()
{
    return 0;
}

void startTimeMeasurement2(void (*timerOverflowCallback)(void))
{
}
uint16_t getTime2()
{
    return 0;
}
uint16_t stopTimeMeasurement2()
{
    return 0;
}


