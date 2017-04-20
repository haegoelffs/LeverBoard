#include "system.h"
#include "leverboardHardware.h"

#include <stdint.h>
#include <avr/io.h>

// PWM:

// CALCULATIONS:
// PWM Frequenz f_pwm: 25kHz
// Taktfrequenz uC f_uC: 16MHz
// PWM Auflösung A_pwm
// Faktor prescaler F_pre

// f_uC/F_pre = f_pwm*A_pwm
// --> F_pre = f_uC/(f_pwm*A_pwm)
// --> A_pwm = f_uC/(f_pwm*F_pre)

// A_pwm = 256
// --> F_pre = f_uC/(f_pwm*A_pwm) = 2.5 (Prescaler Faktor von 2.5 wird nicht unterstützt)

// f_pwm = 40kHz, A_pwm = 256
// --> F_pre = f_uC/(f_pwm*A_pwm) = 1.56

// F_pre = 8
// --> A_pwm = f_uC/(f_pwm*F_pre) = 80
#define A_pwm 80

// F_pre = 8, A_pwm=256
// --> f_pwm = f_uC/(F_pre*A_pwm) = 7.8kHz

// REGISTERS:
// TCNT0 timer/counter 1
// OCR0A OCR0B output/compare (Vergleichswerte des Timers 1 (wenn gleich, wird gesetzt))
// OCF0A OCF0B output compare flag (gesetzt wenn timer 1 = OCR0A/OCR0B)
// OC0x (OC0A/OC0B) output compare pins
#define COMPARE_VALUE OCR0A

// COM0x1:0 compare output mode
//  -definieren des states von OC0x bei match
//  -definieren der output source von OC0x

// fast PWM: timer zählt immer von 0 bis TOP (entweder 0xFF WGM2:0 = 3, oder OCR0A wenn WGM2:0 = 7)
//  -non inverting: ab match auf BOTTOM
//  -invertinmg: ab match auf TOP <-- benötigt


// TCCR0A – Timer/Counter Control Register A
//  |7      |6      |5      |4      |3  |2  |1      |0      |
//  |COM0A1 |COM0A0 |COM0B1 |COM0B0 |-  |-  |WGM01  |WGM00  |

// Compare Output Mode, non PWM Mode: Table 16-2 p.126
// Normal port operation, OC0A disconnected
// COM0A1 = 0
// OOM0A0 = 0

// Compare Output Mode, Fast PWM Mode: Table 16-3 p.126
// inverting fast pwm (Set OC0A on Compare Match, clear OC0A at BOTTOM)
// COM0A1 = 1
// OOM0A0 = 1

// Waveform Generation Mode Bit Description: Table 16-8 p.128
// Normal TOP = MAX (0xFF)
// WGM02 = 0 (TCCR0B register)
// WGM01 = 0
// WGM00 = 0

// Waveform Generation Mode Bit Description: Table 16-8 p.128
// fast PWM TOP = MAX (0xFF)
// WGM2 = 0 (TCCR0B register)
// WGM1 = 1
// WGM0 = 1


// TCCR0B – Timer/Counter Control Register B p.129
//  |7     |6       |5  |4  |3      |2      |1      |0      |
//  |FOC0A |FOC0B   |-  |-  |WGM02  |CS02   |CS01   |CS00   |

// Clock Select Bit Description Table 16-9 p.130
// clk/8
// CS02 = 0
// CS01 = 1
// CS00 = 0


// TIMSK0 – Timer/Counter Interrupt Mask Register p.131
//  |7  |6  |5  |4  |3  |2      |1      |0      |
//  |-  |-  |-  |-  |-  |OCIE0B |OCIE0A |TOIE0  |
// OCIE0B = 1: Timer/Counter Compare Match B interrupt is enabled
// OCIE0A = 1: Timer/Counter Compare Match A interrupt is enabled
// OCIE0B = 1: Timer/Counter0 Overflow Interrupt Enable

// TIFR0 – Timer/Counter 0 Interrupt Flag Register p.131
//  |7  |6  |5  |4  |3  |2      |1      |0      |
//  |-  |-  |-  |-  |-  |OCF0B  |OCF0A  |TOV0   |
// OCF0B: Timer/Counter 0 Output Compare B Match Flag. Gesetzt falls OCR0B=TCNT0
// OCF0A: Timer/Counter 0 Output Compare A Match Flag. Gesetzt falls OCR0A=TCNT0
// TOV0: Timer/Counter0 Overflow Flag

private void initTimer1(void)
{
	// Clear Timer on Compare (CTC) mode (WGM02:0 = 2)
	// TCNT0 = counter value
	// OCR0A = Value A to compare the timer
	// OCR0A = Value B to compare the timer

	// TIMSK0 Bit 2 (OCIE0B) =  Timer/Counter Output Compare Match B Interrupt Enable
	// TIMSK0 Bit 1 (OCIE0A) =  Timer/Counter Output Compare Match A Interrupt Enable

	// OCF2A = 1 enable interrupt when max. value reaches
	// TCCR0A
	// 7:6

	// Timer 0 konfigurieren
	// CTC Modus (p. 128)
	TCCR0A |= (1<<WGM01);

	// clock select
	TCCR0B |= (1<<CS02);
	TCCR0B |= (1<<CS00);
	// prescaler 1024
	// f = 16Mhz/1024 = 15.625kHz
	// T = 1/12.9kHz = 64us

	// value to compare the timer
	// 156*64us = 9.984ms
	OCR0A = 156;

	// acitvating compare interrupt
	TIMSK0 |= (1<<OCIE0A);

	// acitvating global interrupts
	sei();
}

void initPWM()
{

}

void initSoftPWM()
{
    // timer 1 in CTC mode
    TCCR0A |=(1<<WGM10)

    // max. value (CTC mode)
    OCRA = 80;

	// set up timer with prescaler = 64 and PWM, Phase and frequency correct mode
	TCCR1B |= (1 << WGM13)|(1 << CS11)|(1 << CS10);

	// initialize counter
	TCNT1 = 0;

	// initialize compare value
	OCR1A = 10000;

	//TIMSK1 |= (1 << OCIE1A)|(1<<TOIE1);
	TIMSK1 |= (1 << OCIE1A);
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

    char newCompareValue = dutyCycle*A_pwm/100;
    COMPARE_VALUE = newCompareValue;
}

void changePhaseState(char phase, enum phaseState state)
{
}

// timers
void startAfterMs(uint16_t ms, function callback)
{
}

// analog values
void initAnalog()
{
}

void registerVoltageZeroCrossingListener(function callback)
{
}
void setVoltageZeroCrossingPhase(char phase)
{
}

void startIntegration(char limit, function callback)
{
}

char readBatteryVoltage()
{
}
char readPhaseCurrnet(char phase)
{
}
char readPhaseVoltage(char voltage)
{
}
char readInterfaceSensorsVoltage()
{
}

void init_adc()
{
    // die Versorgungsspannung AVcc als Referenz wählen:
    ADMUX = (1<<REFS0);
    // Bit ADFR ("free running") in ADCSRA steht beim Einschalten
    // schon auf 0, also single conversion
    ADCSRA = (1<<ADPS1) | (1<<ADPS0);     // Frequenzvorteiler
    ADCSRA |= (1<<ADEN);                  // ADC aktivieren

    /* nach Aktivieren des ADC wird ein "Dummy-Readout" empfohlen, man liest
       also einen Wert und verwirft diesen, um den ADC "warmlaufen zu lassen" */

    ADCSRA |= (1<<ADSC);                  // eine ADC-Wandlung
    while (ADCSRA & (1<<ADSC) )           // auf Abschluss der Konvertierung warten
    {
    }
    /* ADCW muss einmal gelesen werden, sonst wird Ergebnis der nächsten
       Wandlung nicht übernommen. */
    (void) ADCW;

    // 01 AVCC as reference
    // 0 result right-adjusted
    // 00000 ADC0 Single Ended Input
    ADMUX = 0b01000000;

    // 1 enable adc
    // 0 start conversion
    // 0 free running mode
    // 0 setted when conversion finished
    // 0 adc interrupt enable
    // 111 prescaler 128 --> 16MHz / 128 = 125kHz
    ADCSRA = 0b11100111;
}

short read_adc()
{

// Kanal waehlen, ohne andere Bits zu beeinflußen
    ADMUX = (ADMUX & ~(0x1F)) | (channel & 0x1F);
    ADCSRA |= (1<<ADSC);            // eine Wandlung "single conversion"
    while (ADCSRA & (1<<ADSC) )     // auf Abschluss der Konvertierung warten
    {
    }
    return ADCW;


    // start conversion
    ADCSRA = ADCSRA | (1<<6);

    while(!(ADCSRA & (1<<6)))
    {
        // waiting for conversion
    }

    return ADC;
}

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
}

// log
void initUART();
void logMsg(char *msg);
