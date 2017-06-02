/** Calculation:
PWM Frequenz f_pwm: 25kHz
Taktfrequenz uC f_uC: 16MHz
PWM Auflösung RESOLUTION_PWM
Faktor prescaler F_pre

T_pwm = 1/(f_uC/F_pre) * RESOLUTION_PWM = F_pre/f_uC * RESOLUTION_PWM
--> f_pwm = 1/T_pwm = f_uC/(F_pre * RESOLUTION_PWM)
--> F_pre * RESOLUTION_PWM = f_uC/f_pwm = 640

 Berechnung 1:
    F_pre = 1
    RESOLUTION_PWM = 2¹⁰ = 1024
    Modus: Phase Correct PWM Mode --> f_pwm * 1/2
    T_pwm = 1/(16e6) * 2¹⁰ * 2 = 128us
    f_pwm = 1/T_pwm = 7.8kHz

Berechnung 2:
    F_pre = 1
    f_pwm = 25kHz
    Modus: Phase Correct PWM Mode --> f_pwm * 1/2
    RESOLUTION_PWM = f_uC/(2*f_pwm * F_pre) = 320

 */
//#define RESOLUTION_PWM ((uint32_t)1024)
#define RESOLUTION_PWM ((uint16_t)320)
#define FACTOR_PRESCALER 1

/** TCCR1A – Timer/Counter 1 Control Register A p.154
    |7      |6      |5      |4      |3      |2      |1      |0      |
    |COM1A1 |COM1A0 |COM1B1 |COM1B0 |COM1C1 |COM1C0 |WGM11  |WGM10  |

Bit 7:6 – COMnA1:0: Compare Output Mode for Channel A
Bit 5:4 – COMnB1:0: Compare Output Mode for Channel B
Bit 3:2 – COMnC1:0: Compare Output Mode for Channel C
kontrolliert Verhalten der output compare pins (OCnA, OCnB, OCnC):

COMnA0 = 0 & COMnA1 = 0:
Normal port operation, OCnA/OCnB/OCnC disconnected --> ausschalten entsprechender phase

COMnA0 = 1 & COMnA1 = 1:
Set OCnA/OCnB/OCnC on compare match when up-counting
Clear OCnA/OCnB/OCnC on compare match when downcounting

Bit 1:0 – WGMn1:0: Waveform Generation Mode
kontrolliert mit bits WGMn3:2 in TCCRnB den Waveform Generation Mode (Fats PWM, CTC etc.)

PWM, Phase Correct, 10-bit:
TOP = 0x03FF = 1024
WGM3:0 = 0011

PWM, Phase Correct:
TOP = ICRn
WGM3:0 = 1010
*/
#define PWM_HS_CONTROLL_REGISTER_A TCCR4A
#define PWM_HS_CONTROLL_REGISTER_A_value ((1<<COM4A1) | (1<<COM4A0) | (1<<COM4B1) | (1<<COM4B0) | (1<<COM4C1) | (1<<COM4C0) | (1<<WGM41) | (0<<WGM40))

#define PWM_LS_CONTROLL_REGISTER_A TCCR5A
#define PWM_LS_CONTROLL_REGISTER_A_value ((1<<COM5A1) | (1<<COM5A0) | (1<<COM5B1) | (1<<COM5B0) | (1<<COM5C1) | (1<<COM5C0) | (1<<WGM51) | (0<<WGM50))

#define PWM_HS_A_OFF (PWM_HS_CONTROLL_REGISTER_A &= ~((1<<COM4A1) | (1<<COM4A0)))
#define PWM_HS_A_ON (PWM_HS_CONTROLL_REGISTER_A |= ((1<<COM4A1) | (1<<COM4A0)))
#define PWM_HS_B_OFF (PWM_HS_CONTROLL_REGISTER_A &= ~((1<<COM4B1) | (1<<COM4B0)))
#define PWM_HS_B_ON (PWM_HS_CONTROLL_REGISTER_A |= ((1<<COM4B1) | (1<<COM4B0)))
#define PWM_HS_C_OFF (PWM_HS_CONTROLL_REGISTER_A &= ~((1<<COM4C1) | (1<<COM4C0)))
#define PWM_HS_C_ON (PWM_HS_CONTROLL_REGISTER_A |= ((1<<COM4C1) | (1<<COM4C0)))

/*#define PWM_LS_C_OFF (PWM_LS_CONTROLL_REGISTER_A &= ~((1<<COM5A1) | (1<<COM5A0)))
#define PWM_LS_C_ON (PWM_LS_CONTROLL_REGISTER_A |= ((1<<COM5A1) | (1<<COM5A0)))
#define PWM_LS_B_OFF (PWM_LS_CONTROLL_REGISTER_A &= ~((1<<COM5B1) | (1<<COM5B0)))
#define PWM_LS_B_ON (PWM_LS_CONTROLL_REGISTER_A |= ((1<<COM5B1) | (1<<COM5B0)))
#define PWM_LS_A_OFF (PWM_LS_CONTROLL_REGISTER_A &= ~((1<<COM5C1) | (1<<COM5C0)))
#define PWM_LS_A_ON (PWM_LS_CONTROLL_REGISTER_A |= ((1<<COM5C1) | (1<<COM5C0)))*/

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
*/
#define PWM_HS_CONTROLL_REGISTER_B TCCR4B
#define PWM_HS_CONTROLL_REGISTER_B_value ((0<<ICNC4) | (0<<ICES4) | (1<<WGM43) | (0<<WGM42) | (0<<CS42) | (0<<CS41) | (1<<CS40))

#define PWM_LS_CONTROLL_REGISTER_B TCCR5B
#define PWM_LS_CONTROLL_REGISTER_B_value ((0<<ICNC5) | (0<<ICES5) | (1<<WGM53) | (0<<WGM52) | (0<<CS52) | (0<<CS51) | (1<<CS50))

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

/** OCR1AH and OCR1AL – Output Compare Register 1 A p.159
|7      |6      |5      |4      |3      |2      |1      |0      |
|OCR1A15|OCR1A14|OCR1A13|OCR1A12|OCR1A11|OCR1A10|OCR1A9 |OCR1A8 |  OCR1AH

|7      |6      |5      |4      |3      |2      |1      |0      |
|OCR1A7 |OCR1A6 |OCR1A5 |OCR1A4 |OCR1A3 |OCR1A2 |OCR1A1 |OCR1A0 |  OCR1AL

Vergleichswert Timer 1 Kanal A.
*/
#define PWM_HS_A_COMPARE_VALUE_HIGH OCR4AH
#define PWM_HS_A_COMPARE_VALUE_LOW OCR4AL
#define PWM_HS_B_COMPARE_VALUE_HIGH OCR4BH
#define PWM_HS_B_COMPARE_VALUE_LOW OCR4BL
#define PWM_HS_C_COMPARE_VALUE_HIGH OCR4CH
#define PWM_HS_C_COMPARE_VALUE_LOW OCR4CL

#define PWM_LS_C_COMPARE_VALUE_HIGH OCR5AH
#define PWM_LS_C_COMPARE_VALUE_LOW OCR5AL
#define PWM_LS_B_COMPARE_VALUE_HIGH OCR5BH
#define PWM_LS_B_COMPARE_VALUE_LOW OCR5BL
#define PWM_LS_A_COMPARE_VALUE_HIGH OCR5CH
#define PWM_LS_A_COMPARE_VALUE_LOW OCR5CL

/** ICR1H and ICR1L – Input Capture Register 1 p.161
|7      |6      |5      |4      |3      |2      |1      |0      |
|ICR115 |ICR114 |ICR113 |ICR112 |ICR111 |ICR110 |ICR109 |ICR108 |  ICR1H

|7      |6      |5      |4      |3      |2      |1      |0      |
|ICR107 |ICR106 |ICR105 |ICR104 |ICR103 |ICR102 |ICR101 |ICR100 |  ICR1L

Wird mit Counter TCNTn erhöht, wenn event an ICPn Pin anliegt.
*/
#define PWM_HS_MAX_VALUE_HIGH ICR4H
#define PWM_HS_MAX_VALUE_LOW ICR4L

#define PWM_LS_MAX_VALUE_HIGH ICR5H
#define PWM_LS_MAX_VALUE_LOW ICR5L

/** TIMSK1 – Timer/Counter 1 Interrupt Mask Register
|7      |6      |5      |4      |3      |2      |1      |0      |
|-      |-      |ICIE1  |-      |OCIE1C |OCIE1B |OCIE1A |TOIE1  |

Bit 5 – ICIEn: Timer/Countern, Input Capture Interrupt Enable
Bit 3 – OCIEnC: Timer/Countern, Output Compare C Match Interrupt Enable
Bit 2 – OCIEnB: Timer/Countern, Output Compare B Match Interrupt Enable
Bit 1 – OCIEnA: Timer/Countern, Output Compare A Match Interrupt Enable
Bit 0 – TOIEn: Timer/Countern, Overflow Interrupt Enable
*/

/** TIFR1 – Timer/Counter1 Interrupt Flag Register
|7      |6      |5      |4      |3      |2      |1      |0      |
|-      |-      |ICF1   |-      |OCF1C  |OCF1B  |OCF1A  |TOV1   |

Flags des entsprechenden Interrupts.
*/

/** GPIO Ports
*/
#define TRISTATE_INPUT 0
#define TRISTATE_OUTPUT 1

#define PORT_HS_PWM PORTH
#define TRISTATE_HS_PWM DDRH
#define PIN_HS_A_PWM 3
#define PIN_HS_B_PWM 4
#define PIN_HS_C_PWM 5

#define PORT_LS_PWM PORTL
#define TRISTATE_LS_PWM DDRL
#define PIN_LS_C_PWM 3
#define PIN_LS_B_PWM 4
#define PIN_LS_A_PWM 5

#define PWM_LS_A_OFF (PORT_LS_PWM &= ~(1<<PIN_LS_A_PWM))
#define PWM_LS_A_ON (PORT_LS_PWM |= (1<<PIN_LS_A_PWM))
#define PWM_LS_B_OFF (PORT_LS_PWM &= ~(1<<PIN_LS_B_PWM))
#define PWM_LS_B_ON (PORT_LS_PWM |= (1<<PIN_LS_B_PWM))
#define PWM_LS_C_OFF (PORT_LS_PWM &= ~(1<<PIN_LS_C_PWM))
#define PWM_LS_C_ON (PORT_LS_PWM |= (1<<PIN_LS_C_PWM))

/*#define PWM_HS_A_OFF (PORT_HS_PWM &= ~(1<<PIN_HS_A_PWM))
#define PWM_HS_A_ON (PORT_HS_PWM |= (1<<PIN_HS_A_PWM))
#define PWM_HS_B_OFF (PORT_HS_PWM &= ~(1<<PIN_HS_B_PWM))
#define PWM_HS_B_ON (PORT_HS_PWM |= (1<<PIN_HS_B_PWM))
#define PWM_HS_C_OFF (PORT_HS_PWM &= ~(1<<PIN_HS_C_PWM))
#define PWM_HS_C_ON (PORT_HS_PWM |= (1<<PIN_HS_C_PWM))*/


// INCLUDES:
#include "system.h"
#include "logger.h"

#include <stdint.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

uint8_t phaseState = 6; // power off all

// pwm
void initPWM()
{
    logMsgLn("Init PWM...");

    // config GPIOs
    TRISTATE_HS_PWM |= ((TRISTATE_OUTPUT<<PIN_HS_A_PWM) | (TRISTATE_OUTPUT<<PIN_HS_B_PWM) | (TRISTATE_OUTPUT<<PIN_HS_C_PWM));
    TRISTATE_LS_PWM |= ((TRISTATE_OUTPUT<<PIN_LS_A_PWM) | (TRISTATE_OUTPUT<<PIN_LS_B_PWM) | (TRISTATE_OUTPUT<<PIN_LS_C_PWM));

    // config timers
    PWM_HS_CONTROLL_REGISTER_A |= PWM_HS_CONTROLL_REGISTER_A_value;
    //PWM_LS_CONTROLL_REGISTER_A |= PWM_LS_CONTROLL_REGISTER_A_value;

    PWM_HS_CONTROLL_REGISTER_B |= PWM_HS_CONTROLL_REGISTER_B_value;
    //PWM_LS_CONTROLL_REGISTER_B |= PWM_LS_CONTROLL_REGISTER_B_value;

    PWM_HS_MAX_VALUE_HIGH = (char)(RESOLUTION_PWM>>8);
    PWM_HS_MAX_VALUE_LOW = (char)(RESOLUTION_PWM);
    //PWM_LS_MAX_VALUE_HIGH = (char)(RESOLUTION_PWM>>8);
    //PWM_LS_MAX_VALUE_LOW = (char)(RESOLUTION_PWM);
}

/** Changes the output channels for the pwm.
    Input:
    state = 0: A heavyside, C lowside
    state = 1: B heavyside, C lowside
    state = 2: B heavyside, A lowside
    state = 3: C heavyside, A lowside
    state = 4: C heavyside, B lowside
    state = 5: A heavyside, B lowside
    state > 5: power off all channels
**/
void changePhaseState(uint8_t newPhaseState)
{
    PWM_HS_A_OFF;
    PWM_HS_B_OFF;
    PWM_HS_C_OFF;
    PWM_LS_A_OFF;
    PWM_LS_B_OFF;
    PWM_LS_C_OFF;

    switch(newPhaseState)
    {
        case 0:
            PWM_HS_A_ON;
            PWM_LS_C_ON;
            break;

        case 1:
            PWM_HS_B_ON;
            PWM_LS_C_ON;
            break;

        case 2:
            PWM_HS_B_ON;
            PWM_LS_A_ON;
            break;

        case 3:
            PWM_HS_C_ON;
            PWM_LS_A_ON;
            break;

        case 4:
            PWM_HS_C_ON;
            PWM_LS_B_ON;
            break;

        case 5:
            PWM_HS_A_ON;
            PWM_LS_B_ON;
            break;

        default:
            break;
    }

    phaseState = newPhaseState;
}

uint8_t getPhaseState()
{
    return phaseState;
}

/** Sets the dutycycle of the pwm.
    Input:
    0 <= dutyCycle <= 100 every handed value > 100 will be interpreted as 100
 **/
void setPWMDutyCycle(uint8_t dutyCycle)
{
    if(dutyCycle > 100)
    {
        dutyCycle = 100;
    }


    dutyCycle = abs((int16_t)dutyCycle-100);
    uint16_t temp = dutyCycle*RESOLUTION_PWM; // max.: 100*320 = 32000 = 0b0111 1101 0000 0000 --> 16Bit
    uint16_t newCompareValue = temp/((uint16_t)100);

    PWM_HS_A_COMPARE_VALUE_HIGH = (char)(newCompareValue>>8);
    PWM_HS_A_COMPARE_VALUE_LOW = (char)newCompareValue;
    PWM_HS_B_COMPARE_VALUE_HIGH = (char)(newCompareValue>>8);
    PWM_HS_B_COMPARE_VALUE_LOW = (char)newCompareValue;
    PWM_HS_C_COMPARE_VALUE_HIGH = (char)(newCompareValue>>8);
    PWM_HS_C_COMPARE_VALUE_LOW = (char)newCompareValue;

    PWM_LS_A_COMPARE_VALUE_HIGH = (char)(newCompareValue>>8);
    PWM_LS_A_COMPARE_VALUE_LOW = (char)newCompareValue;
    PWM_LS_B_COMPARE_VALUE_HIGH = (char)(newCompareValue>>8);
    PWM_LS_B_COMPARE_VALUE_LOW = (char)newCompareValue;
    PWM_LS_C_COMPARE_VALUE_HIGH = (char)(newCompareValue>>8);
    PWM_LS_C_COMPARE_VALUE_LOW = (char)newCompareValue;
}

