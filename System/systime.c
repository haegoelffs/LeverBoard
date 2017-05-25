
/** calculations:
Systime:
    Prescaler = 1/64
        1/(16e6/1024) = 64us
        - Zeitbereich:
        64us * 2⁸ = 16.384ms
        64us * 2¹⁶ = 4.194s
        64us * 2³² = 274'877.9s = 4'581.3min = 76h

    Max. Time = +/- 10ms
        10ms/64us = 156.25
        -> max. timer value = 156
        -> max. zeitbereich = 65us * 156 = 10.14ms
 */

/** REGISTERS:
 TCNT0 timer/counter 1
 OCR0A output/compare (Vergleichswerte des Timers 1). Bei Modus "Fast PWM" obere Grenze.
 OCR0B output/compare (Vergleichswerte des Timers 1)*/
#define SYSTIME_TIMER_VALUE TCNT0
#define SYSTIME_MAX_VALUE OCR0A

/** TCCR0A – Timer/Counter Control Register A p.126
  |7      |6      |5      |4      |3  |2  |1      |0      |
  |COM0A1 |COM0A0 |COM0B1 |COM0B0 |-  |-  |WGM01  |WGM00  |

 COM0x1:0 compare output mode
  -definieren des states von OC0x bei match
  -definieren der output source von OC0x

 Compare Output Mode, non PWM Mode: Table 16-2 p.126
 Normal port operation, OC0A disconnected (für soft pwm)
 COM0A1 = 0
 COM0A0 = 0

 Compare Output Mode, Fast PWM Mode: Table 16-3 p.126
 inverting fast pwm (Set OC0A on Compare Match, clear OC0A at BOTTOM)
 COM0A1 = 1
 COM0A0 = 1

 Waveform Generation Mode Bit Description: Table 16-8 p.128
 CTC TOP = OCR0A
 WGM02 = 0 (TCCR0B register)
 WGM01 = 1
 WGM00 = 0

 Waveform Generation Mode Bit Description: Table 16-8 p.128
 PWM, Phase Correct TOP = OCR0A
 WGM2 = 1 (TCCR0B register)
 WGM1 = 0
 WGM0 = 1

 Waveform Generation Mode Bit Description: Table 16-8 p.128
 fast PWM TOP = OCR0A
 WGM2 = 1 (TCCR0B register)
 WGM1 = 1
 WGM0 = 1*/
#define SYSTIME_CONTROL_REGISTER_A TCCR0A
#define SYSTIME_CONTROL_REGISTER_A_value ((0<<WGM00) | (1<<WGM01) | (0<<COM0A1) | (0<<COM0A0) | (0<<COM0B1) | (0<<COM0B0))


/** TCCR0B – Timer/Counter Control Register B p.129
  |7     |6       |5  |4  |3      |2      |1      |0      |
  |FOC0A |FOC0B   |-  |-  |WGM02  |CS02   |CS01   |CS00   |

 Clock Select Bit Description Table 16-9 p.130
 clk/1024
 CS02 = 1
 CS01 = 0
 CS00 = 1

 Force Output Compare A/B
 In PWM Mode zu kompatibilitätszwecken mit 0 beschrieben
 FOC0A = 0
 FOC0B = 0*/
#define SYSTIME_CONTROL_REGISTER_B TCCR0B
#define SYSTIME_CONTROL_REGISTER_B_value ((0<<WGM02) | (1<<CS00) | (0<<CS01) | (1<<CS02) | (0<<FOC0A) | (0<<FOC0B))

/** TIMSK0 – Timer/Counter Interrupt Mask Register p.131
  |7  |6  |5  |4  |3  |2      |1      |0      |
  |-  |-  |-  |-  |-  |OCIE0B |OCIE0A |TOIE0  |
 OCIE0B = 1: Timer/Counter Compare Match B interrupt is enabled
 OCIE0A = 1: Timer/Counter Compare Match A interrupt is enabled
 TOIE0 = 1: Timer/Counter0 Overflow Interrupt Enable*/
#define SYSTIME_ENABLE_INTERRUPTS TIMSK0
#define SYSTIME_ENABLE_INTERRUPTS_value ((0<<OCIE0B) | (1<<OCIE0A) | (0<<TOIE0))

/** TIFR0 – Timer/Counter 0 Interrupt Flag Register p.131
  |7  |6  |5  |4  |3  |2      |1      |0      |
  |-  |-  |-  |-  |-  |OCF0B  |OCF0A  |TOV0   |
 OCF0B: Timer/Counter 0 Output Compare B Match Flag. Gesetzt falls OCR0B=TCNT0
 OCF0A: Timer/Counter 0 Output Compare A Match Flag. Gesetzt falls OCR0A=TCNT0
 TOV0: Timer/Counter0 Overflow Flag*/

 // interrupts
 #define SYSTIME_TICK_ISR TIMER0_COMPA_vect

// includes
#include "system.h"
#include "logger.h"

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

// variables
volatile uint16_t time_ms;

void initSystime()
{
    logMsgLn("Init Systime...");

    SYSTIME_CONTROL_REGISTER_A |= SYSTIME_CONTROL_REGISTER_A_value;
    SYSTIME_CONTROL_REGISTER_B |= SYSTIME_CONTROL_REGISTER_B_value;
    SYSTIME_MAX_VALUE = 156;

    SYSTIME_ENABLE_INTERRUPTS |= SYSTIME_ENABLE_INTERRUPTS_value;
}

uint16_t getSystimeMs()
{
    return time_ms;
}

// interrupts
ISR (SYSTIME_TICK_ISR)
{
    time_ms = time_ms + 10;
}
