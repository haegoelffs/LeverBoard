#ifndef LEVERBOARDHARDWARE_H_INCLUDED
#define LEVERBOARDHARDWARE_H_INCLUDED

// __GPIOs
#define TRISTATE_INPUT 0
#define TRISTATE_OUTPUT 1

#define PORT_LEDS PORTA
#define TRISTATE_LEDS DDRA
#define PIN_LED0 0
#define PIN_LED1 1
#define PIN_LED2 2
#define PIN_LED3 3

#define PORT_PIEZO PORTA
#define TRISTATE_PIEZO DDRA
#define PIN_PIEZO 4

#define PORT_ENABLE_DRIVER PORTA
#define TRISTATE_ENABLE_DRIVER DDRA
#define PIN_ENABLE_DRIVER 5

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
// __PWM________________________________________________________

// CALCULATIONS:
// PWM Frequenz f_pwm: 25kHz
// Taktfrequenz uC f_uC: 16MHz
// PWM Auflösung RESOLUTION_PWM
// Faktor prescaler F_pre

// f_uC/F_pre = f_pwm*RESOLUTION_PWM
// --> F_pre = f_uC/(f_pwm*RESOLUTION_PWM)
// --> RESOLUTION_PWM = f_uC/(f_pwm*F_pre)

// RESOLUTION_PWM = 256
// --> F_pre = f_uC/(f_pwm*RESOLUTION_PWM) = 2.5

// f_pwm = 40kHz, RESOLUTION_PWM = 256
// --> F_pre = f_uC/(f_pwm*RESOLUTION_PWM) = 1.56

// F_pre = 8
// --> RESOLUTION_PWM = f_uC/(f_pwm*F_pre) = 80

// F_pre = 8, RESOLUTION_PWM=256
// --> f_pwm = f_uC/(F_pre*RESOLUTION_PWM) = 7.8kHz (zu tiefe Frequenz. Bis auf weiteres genommen)

// F_pre = 8, phase correct --> needs two timer cycles! --> f_pwm_pc = 2*f_pwm = 50kHz
// --> RESOLUTION_PWM = f_uC/(2*f_pwm*F_pre) = 40

// F_pre = 8, f_pwm = 10kHz, phase correct --> needs two timer cycles! --> f_pwm_pc = 2*f_pwm = 20kHz
// --> RESOLUTION_PWM = f_uC/(2*f_pwm*F_pre) = 100
#define RESOLUTION_PWM 100
#define FACTOR_PRESCALER 8

// REGISTERS:
// TCNT0 timer/counter 1
#define VALUE_PWM_TIMER TCNT0

// OCR0A output/compare (Vergleichswerte des Timers 1). Bei fast PWM obere Grenze.
#define MAX_VALUE_PWM_TIMER OCR0A

// OCR0B output/compare (Vergleichswerte des Timers 1)
#define COMPARE_VALUE_PWM_TIMER OCR0B

// OCF0A OCF0B output compare flag (gesetzt wenn timer 1 = OCR0A/OCR0B)
// OC0x (OC0A/OC0B) output compare pins


// TCCR0A – Timer/Counter Control Register A p.126
//  |7      |6      |5      |4      |3  |2  |1      |0      |
//  |COM0A1 |COM0A0 |COM0B1 |COM0B0 |-  |-  |WGM01  |WGM00  |
#define CONTROL_REGISTER_A_PWM_TIMER TCCR0A

// COM0x1:0 compare output mode
//  -definieren des states von OC0x bei match
//  -definieren der output source von OC0x

// Compare Output Mode, non PWM Mode: Table 16-2 p.126
// Normal port operation, OC0A disconnected (für soft pwm)
// COM0A1 = 0
// COM0A0 = 0

// Compare Output Mode, Fast PWM Mode: Table 16-3 p.126
// inverting fast pwm (Set OC0A on Compare Match, clear OC0A at BOTTOM)
// COM0A1 = 1
// COM0A0 = 1


// fast PWM: timer zählt immer von 0 bis TOP (entweder 0xFF WGM2:0 = 3, oder OCR0A wenn WGM2:0 = 7)
//  -non inverting: ab match auf BOTTOM
//  -invertinmg: ab match auf TOP <-- benötigt

// Waveform Generation Mode Bit Description: Table 16-8 p.128
// CTC TOP = OCR0A
// WGM02 = 0 (TCCR0B register)
// WGM01 = 1
// WGM00 = 0
#define CTC_SOFT_PWM_A ((0<<WGM00) | (1<<WGM01) | (0<<COM0A1) | (0<<COM0A0) | (0<<COM0B1) | (0<<COM0B0))

// Waveform Generation Mode Bit Description: Table 16-8 p.128
// PWM, Phase Correct TOP = OCR0A
// WGM2 = 1 (TCCR0B register)
// WGM1 = 0
// WGM0 = 1
#define PC_SOFT_PWM_A ((1<<WGM00) | (0<<WGM01) | (0<<COM0A1) | (0<<COM0A0) | (0<<COM0B1) | (0<<COM0B0))

// Waveform Generation Mode Bit Description: Table 16-8 p.128
// fast PWM TOP = OCR0A
// WGM2 = 1 (TCCR0B register)
// WGM1 = 1
// WGM0 = 1
#define FAST_SOFT_PWM_A ((1<<WGM00) | (1<<WGM01) | (0<<COM0A1) | (0<<COM0A0) | (0<<COM0B1) | (0<<COM0B0))

// TCCR0B – Timer/Counter Control Register B p.129
//  |7     |6       |5  |4  |3      |2      |1      |0      |
//  |FOC0A |FOC0B   |-  |-  |WGM02  |CS02   |CS01   |CS00   |
#define CONTROL_REGISTER_B_PWM_TIMER TCCR0B

// Clock Select Bit Description Table 16-9 p.130
// clk/8
// CS02 = 0
// CS01 = 1
// CS00 = 0

// Force Output Compare A/B
// In PWM Mode zu kompatibilitätszwecken mit 0 beschrieben
// FOC0A = 0
// FOC0B = 0
#define FAST_SOFT_PWM_PRESC_8_B ((1<<WGM02) | (0<<CS00) | (1<<CS01) | (0<<CS02) | (0<<FOC0A) | (0<<FOC0B))
#define FAST_SOFT_PWM_PRESC_1024_B ((1<<WGM02) | (1<<CS00) | (0<<CS01) | (1<<CS02) | (0<<FOC0A) | (0<<FOC0B))
#define CTC_SOFT_PWM_PRESC_1024_B ((0<<WGM02) | (1<<CS00) | (0<<CS01) | (1<<CS02) | (0<<FOC0A) | (0<<FOC0B))
#define PC_SOFT_PWM_PRESC_8_B ((1<<WGM02) | (0<<CS00) | (1<<CS01) | (0<<CS02) | (0<<FOC0A) | (0<<FOC0B))

// TIMSK0 – Timer/Counter Interrupt Mask Register p.131
//  |7  |6  |5  |4  |3  |2      |1      |0      |
//  |-  |-  |-  |-  |-  |OCIE0B |OCIE0A |TOIE0  |
// OCIE0B = 1: Timer/Counter Compare Match B interrupt is enabled
// OCIE0A = 1: Timer/Counter Compare Match A interrupt is enabled
// TOIE0 = 1: Timer/Counter0 Overflow Interrupt Enable
#define ENABLE_INTERRUPTS_PWM_TIMER TIMSK0
#define COMPARE_A_B_INTERRUPT ((1<<OCIE0B) | (1<<OCIE0A) | (0<<TOIE0))
#define COMPARE_B_INTERRUPT ((1<<OCIE0B) | (0<<OCIE0A) | (0<<TOIE0))

// TIFR0 – Timer/Counter 0 Interrupt Flag Register p.131
//  |7  |6  |5  |4  |3  |2      |1      |0      |
//  |-  |-  |-  |-  |-  |OCF0B  |OCF0A  |TOV0   |
// OCF0B: Timer/Counter 0 Output Compare B Match Flag. Gesetzt falls OCR0B=TCNT0
// OCF0A: Timer/Counter 0 Output Compare A Match Flag. Gesetzt falls OCR0A=TCNT0
// TOV0: Timer/Counter0 Overflow Flag
#define INTERRUPT_FLAGS_PWM_TIMER TIFR0
#define SET_HIGH_FLAG OCF0B
#define SET_LOW_FLAG OCF0A




// __ANALOG COMPERATOR________________________________________________________
// Vergleicht Input an positivem (AIN0) und negativem (AIN1) Pin und setzt Output ACO
// Voraussetzungen:
// - Analog Comparator Multiplexer Enable bit (ACME in ADCSRB) gesetzt
// - ADC ausgeschaltet (ADEN in ADC-SRA = 0) --> Multiplexer von ADC wird verwendet um umzuschalten auf unterschiedliche Pins
// - Mittels MUX5 & MUX2:0 in ADMUX wird pin für negativen Eingang AIN1 gewähl (Tabelle 25-1 p.265)

// Registers:
// ADCSRB – ADC Control and Status Register B p.266
//  |7      |6      |5      |4      |3      |2      |1      |0      |
//  |ACD    |ACBG   |ACO    |ACI    |ACIE   |ACIC   |ACIS1  |ACIS0  |
#define COMPERATOR_STATUS_REGISTER ADCSRB

// ACD: Analog Comparator Disable
// Wenn auf logisch 1 gesetzt --> Comperator ausgeschaltet
// Kann jederzeit geschaltet werden (Energie sparen etc.)
// Wenn bit veräbdert wird, kann möglicherweise Interrupt von AC ausgelöst werden --> ausschalten

// ACBG: Analog Comparator Bandgap Select
// Wenn = 1, wird die interne Referenzsspannung als positiven Eingang benutzt
// Wenn = 0, wird AIN0 als positiven Eingang benutzt

// ACO: Analog Comparator Output
// AIN0 > AIN1 --> ACO = 1
// AIN0 < AIN1 --> ACO = 0
// Verzögerung von 1 - 2 Clockzyklen (Synchronisation)

// ACI: Analog Comparator Interrupt Flag
// Gesetzt von der Hardware wenn vom AC ein Interrupt ausgelöst wird.

// ACIE: Analog Comparator Interrupt Enable
// Wenn = 1, sind AC Interrupts anktiviert
// Wenn = 0, sind AC Interrupts deaktiviert

// ACIC: Analog Comparator Input Capture Enable
// Wenn = 1, wird der Ausgang des AC mit dem Timer/Counter1 trigger verbunden
// Wenn = 0, keine Verbindung zu Timer/Counter1 trigger

// ACIS1, ACIS0: Analog Comparator Interrupt Mode Select
// Festlegen des Interrupts für Comperator
// Table 25-2 p.267
// ACIS1 = 0, ACIS0 = 0 --> Interrupt bei Output Toggle (sinkende und steigende Flanke)
#define COMPERATOR_STATUS_REGISTER_VALUE ((0<<ACD) | (0<<ACBG) | (1<<ACIE) | (0<<ACIC) | (0<<ACIS1) | (0<<ACIS0))


// DIDR1 – Digital Input Disable Register 1 p.267
//  |7      |6      |5      |4      |3      |2      |1      |0      |
//  |-      |-      |-      |-      |-      |-      |AIN1D  |AIN0D  |
#define COMPERATOR_DIGITAL_INPUT_CONTROLL_REGISTER DIDR1

// AIN1D, AIN0D: AIN1, AIN0 Digital Input Disable
// Wenn = 1, wird der digitale Inputbuffer für die Pins AIN1/0 deaktiviert
// (wird digital immer eine 0 gelsenen --> Energie sparren wenn analog verwendet wird)
// Wenn = 0, digitaler Inputbuffer aktiviert
#define COMPERATOR_DIGITAL_INPUT_CONTROLL_REGISTER_VALUE ((1<<AIN1D) | (1<<AIN0D))



// __SPI________________________________________________________

// Registers:

// SPCR – SPI Control Register
//  |7      |6      |5      |4      |3      |2      |1      |0      |
//  |SPIE   |SPE    |DORD   |MASTR  |CPOL   |CPHA   |SPR1   |SPR0   |

// SPIE: SPI Interrupt Enable

// SPE: SPI Enable

// DORD: Data Order
// Wenn = 1, LSB zuerst
// Wenn = 0, MSB zuerst <--|

// MSTR: Master/Slave Select
// Wenn = 1, --> Mastermode <--|
// Wenn = 0, --> Slavemode
// Falls !SS als Eingang konfiguriert & = 0 während MSTR gesetzt ist, wird MSTR = 0

// CPOL: Clock Polarity
// Wenn = 1, SCK high wenn untätig
// Wenn = 0, SCK low wenn untätig<--|

// CPHA: Clock Phase
// Wenn = 1, sampling findet bei hinterkante von clk statt
// Wenn = 0, sampling findet bei vorderkante von clk statt<--|

// SPR1, SPR0: SPI Clock Rate Select 1 and 0
// Table 21-5 p.198


// SPCR – SPI Control Register
//  |7      |6      |5      |4      |3      |2      |1      |0      |
//  |SPIF   |WCOL   |-      |-      |-      |-      |-      |SPI2X  |

// SPIF: SPI Interrupt Flag
// Gesetzt wenn:
// - serieller Transfer beendet
// - wenn Mastermode, !SS ein Eingang & aus 0 gezogen

// WCOL: Write COLlision Flag
// gesetzt, falls Datenregister SPDR beschrieben wird während Datentransfer
// zurückgesetzt, wenn SPCR gelesen und Datebregister neu beschrieben wird

// SPI2X: Double SPI Speed Bit
// Wenn gesetzt, wird die SCK Frequenz verdoppelt


// SPDR – SPI Data Register
//  |7      |6      |5      |4      |3      |2      |1      |0      |
//  |MSB    |       |       |       |       |       |       |LSB    |

#endif // LEVERBOARDHARDWARE_H_INCLUDED
