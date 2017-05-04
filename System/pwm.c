// __PWM________________________________________________________

/* CALCULATIONS:
 PWM Frequenz f_pwm: 25kHz
 Taktfrequenz uC f_uC: 16MHz
 PWM Auflösung RESOLUTION_PWM
 Faktor prescaler F_pre

 f_uC/F_pre = f_pwm*RESOLUTION_PWM
 --> F_pre = f_uC/(f_pwm*RESOLUTION_PWM)
 --> RESOLUTION_PWM = f_uC/(f_pwm*F_pre)

 RESOLUTION_PWM = 256
 --> F_pre = f_uC/(f_pwm*RESOLUTION_PWM) = 2.5

 f_pwm = 40kHz, RESOLUTION_PWM = 256
 --> F_pre = f_uC/(f_pwm*RESOLUTION_PWM) = 1.56

 F_pre = 8
 --> RESOLUTION_PWM = f_uC/(f_pwm*F_pre) = 80

 F_pre = 8, RESOLUTION_PWM=256
 --> f_pwm = f_uC/(F_pre*RESOLUTION_PWM) = 7.8kHz (zu tiefe Frequenz. Bis auf weiteres genommen)

 F_pre = 8, phase correct --> needs two timer cycles! --> f_pwm_pc = 2*f_pwm = 50kHz
 --> RESOLUTION_PWM = f_uC/(2*f_pwm*F_pre) = 40

 F_pre = 8, f_pwm = 10kHz, phase correct --> needs two timer cycles! --> f_pwm_pc = 2*f_pwm = 20kHz
 --> RESOLUTION_PWM = f_uC/(2*f_pwm*F_pre) = 100*/
#define RESOLUTION_PWM 100
#define FACTOR_PRESCALER 8

/* REGISTERS:
 TCNT0 timer/counter 1
 OCR0A output/compare (Vergleichswerte des Timers 1). Bei fast PWM obere Grenze.
 OCR0B output/compare (Vergleichswerte des Timers 1)*/
#define VALUE_PWM_TIMER TCNT0
#define MAX_VALUE_PWM_TIMER OCR0A
#define COMPARE_VALUE_PWM_TIMER OCR0B

/* TCCR0A – Timer/Counter Control Register A p.126
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

 fast PWM: timer zählt immer von 0 bis TOP (entweder 0xFF WGM2:0 = 3, oder OCR0A wenn WGM2:0 = 7)
  -non inverting: ab match auf BOTTOM
  -invertinmg: ab match auf TOP <-- benötigt

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
#define CONTROL_REGISTER_A_PWM_TIMER TCCR0A
#define CTC_SOFT_PWM_A ((0<<WGM00) | (1<<WGM01) | (0<<COM0A1) | (0<<COM0A0) | (0<<COM0B1) | (0<<COM0B0))
#define PC_SOFT_PWM_A ((1<<WGM00) | (0<<WGM01) | (0<<COM0A1) | (0<<COM0A0) | (0<<COM0B1) | (0<<COM0B0))
#define FAST_SOFT_PWM_A ((1<<WGM00) | (1<<WGM01) | (0<<COM0A1) | (0<<COM0A0) | (0<<COM0B1) | (0<<COM0B0))

/* TCCR0B – Timer/Counter Control Register B p.129
  |7     |6       |5  |4  |3      |2      |1      |0      |
  |FOC0A |FOC0B   |-  |-  |WGM02  |CS02   |CS01   |CS00   |

 Clock Select Bit Description Table 16-9 p.130
 clk/8
 CS02 = 0
 CS01 = 1
 CS00 = 0

 Force Output Compare A/B
 In PWM Mode zu kompatibilitätszwecken mit 0 beschrieben
 FOC0A = 0
 FOC0B = 0*/
#define CONTROL_REGISTER_B_PWM_TIMER TCCR0B
#define FAST_SOFT_PWM_PRESC_8_B ((1<<WGM02) | (0<<CS00) | (1<<CS01) | (0<<CS02) | (0<<FOC0A) | (0<<FOC0B))
#define FAST_SOFT_PWM_PRESC_1024_B ((1<<WGM02) | (1<<CS00) | (0<<CS01) | (1<<CS02) | (0<<FOC0A) | (0<<FOC0B))
#define CTC_SOFT_PWM_PRESC_1024_B ((0<<WGM02) | (1<<CS00) | (0<<CS01) | (1<<CS02) | (0<<FOC0A) | (0<<FOC0B))
#define PC_SOFT_PWM_PRESC_8_B ((1<<WGM02) | (0<<CS00) | (1<<CS01) | (0<<CS02) | (0<<FOC0A) | (0<<FOC0B))

/* TIMSK0 – Timer/Counter Interrupt Mask Register p.131
  |7  |6  |5  |4  |3  |2      |1      |0      |
  |-  |-  |-  |-  |-  |OCIE0B |OCIE0A |TOIE0  |
 OCIE0B = 1: Timer/Counter Compare Match B interrupt is enabled
 OCIE0A = 1: Timer/Counter Compare Match A interrupt is enabled
 TOIE0 = 1: Timer/Counter0 Overflow Interrupt Enable*/
#define ENABLE_INTERRUPTS_PWM_TIMER TIMSK0
#define COMPARE_A_B_INTERRUPT ((1<<OCIE0B) | (1<<OCIE0A) | (0<<TOIE0))
#define COMPARE_B_INTERRUPT ((1<<OCIE0B) | (0<<OCIE0A) | (0<<TOIE0))

/* TIFR0 – Timer/Counter 0 Interrupt Flag Register p.131
  |7  |6  |5  |4  |3  |2      |1      |0      |
  |-  |-  |-  |-  |-  |OCF0B  |OCF0A  |TOV0   |
 OCF0B: Timer/Counter 0 Output Compare B Match Flag. Gesetzt falls OCR0B=TCNT0
 OCF0A: Timer/Counter 0 Output Compare A Match Flag. Gesetzt falls OCR0A=TCNT0
 TOV0: Timer/Counter0 Overflow Flag*/
#define INTERRUPT_FLAGS_PWM_TIMER TIFR0
#define SET_HIGH_FLAG OCF0B
#define SET_LOW_FLAG OCF0A

// GPIOs SOFT PWM:
#define TRISTATE_INPUT 0
#define TRISTATE_OUTPUT 1

#define PORT_PWM1 PORTB
#define PORT_PWM2 PORTB
#define PORT_PWM3 PORTB
#define PORT_PWM4 PORTB
#define PORT_PWM5 PORTH
#define PORT_PWM6 PORTH

#define TRISTATE_PWM1 DDRB
#define TRISTATE_PWM2 DDRB
#define TRISTATE_PWM3 DDRB
#define TRISTATE_PWM4 DDRB
#define TRISTATE_PWM5 DDRH
#define TRISTATE_PWM6 DDRH

#define PIN_PWM1 7
#define PIN_PWM2 6
#define PIN_PWM3 5
#define PIN_PWM4 4
#define PIN_PWM1 6
#define PIN_PWM1 5

#define POWER_ON_A_HIGHSIDE (PORT_PWM1 |= (1<<PIN_PWM1))
#define POWER_OFF_A_HIGHSIDE (PORT_PWM1 &= ~(1<<PIN_PWM1))
#define POWER_ON_A_LOWSIDE (PORT_PWM2 |= (1<<PIN_PWM2))
#define POWER_OFF_A_LOWSIDE (PORT_PWM2 &= ~(1<<PIN_PWM2))

#define POWER_ON_B_HIGHSIDE (PORT_PWM3 |= (1<<PIN_PWM3))
#define POWER_OFF_B_HIGHSIDE (PORT_PWM3 &= ~(1<<PIN_PWM3))
#define POWER_ON_B_LOWSIDE (PORT_PWM4 |= (1<<PIN_PWM4))
#define POWER_OFF_B_LOWSIDE (PORT_PWM4 &= ~(1<<PIN_PWM4))

#define POWER_ON_C_HIGHSIDE (PORT_PWM5 |= (1<<PIN_PWM5))
#define POWER_OFF_C_HIGHSIDE (PORT_PWM5 &= ~(1<<PIN_PWM5))
#define POWER_ON_C_LOWSIDE (PORT_PWM6 |= (1<<PIN_PWM6))
#define POWER_OFF_C_LOWSIDE (PORT_PWM6 &= ~(1<<PIN_PWM6))

// INCLUDES:
#include "system.h"
#include "logger.h"

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

void (*activePhase1)(void);
void (*activePhase2)(void);

// helping functions
void toggle_A_Highside()
{
    PORT_PWM1 ^= (1<<PIN_PWM1);
}

void toggle_A_Lowside()
{
    PORT_PWM2 ^= (1<<PIN_PWM2);
}

void toggle_B_Highside()
{
    PORT_PWM3 ^= (1<<PIN_PWM3);
}

void toggle_B_Lowside()
{
    PORT_PWM4 ^= (1<<PIN_PWM4);
}

void toggle_C_Highside()
{
    PORT_PWM5 ^= (1<<PIN_PWM5);
}

void toggle_C_Lowside()
{
    PORT_PWM6 ^= (1<<PIN_PWM6);
}


// interrupts
ISR (TIMER0_COMPB_vect)
{
    // power on pwm
    activePhase1();
    activePhase2();
}

ISR (TIMER0_COMPA_vect)
{
    // power off pwm
    activePhase1();
    activePhase2();
}

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

    // init gpios for soft pwm
    TRISTATE_PWM1 |= (TRISTATE_OUTPUT<<PIN_PWM1);
    TRISTATE_PWM2 |= (TRISTATE_OUTPUT<<PIN_PWM2);
    TRISTATE_PWM3 |= (TRISTATE_OUTPUT<<PIN_PWM3);
    TRISTATE_PWM4 |= (TRISTATE_OUTPUT<<PIN_PWM4);
    TRISTATE_PWM5 |= (TRISTATE_OUTPUT<<PIN_PWM5);
    TRISTATE_PWM6 |= (TRISTATE_OUTPUT<<PIN_PWM6);
}

void changePhaseState(char state)
{
    // disable pwm interrupts
    ENABLE_INTERRUPTS_PWM_TIMER TIMSK0 &= ((0<<OCIE0B) | (0<<OCIE0A));

    // shutdown all phases
    POWER_OFF_A_HIGHSIDE;
    POWER_OFF_A_LOWSIDE;
    POWER_OFF_B_HIGHSIDE;
    POWER_OFF_B_LOWSIDE;
    POWER_OFF_C_HIGHSIDE;
    POWER_OFF_C_LOWSIDE;

    switch(state)
    {
    case 0:
        activePhase1 = $toggle_A_Highside();
        activePhase2 = $toggle_C_Lowside();
        break;

    case 1:
        activePhase1 = $toggle_B_Highside();
        activePhase2 = $toggle_C_Lowside();
        break;

    case 2:
        activePhase1 = $toggle_B_Highside();
        activePhase2 = $toggle_A_Lowside();
        break;

    case 3:
        activePhase1 = $toggle_C_Highside();
        activePhase2 = $toggle_A_Lowside();
        break;

    case 4:
        activePhase1 = $toggle_C_Highside();
        activePhase2 = $toggle_B_Lowside();
        break;

    case 5:
        activePhase1 = $toggle_A_Highside();
        activePhase2 = $toggle_B_Lowside();
        break;
    }

    // enable pwm interrupts
    ENABLE_INTERRUPTS_PWM_TIMER TIMSK0 |= ((1<<OCIE0B) | (1<<OCIE0A));
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

