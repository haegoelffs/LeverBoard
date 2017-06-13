
/** calculation
Prescaler = clk/64
16e6/64 = 250kHz --> 4us
Conversion time = 13 * 4us = 52us <-- gewählt, da so wie so nur mit 8 Bit Auflösung gearbeitet wird

Prescaler = clk/128
16e6/128 = 125kHz --> 8us
Conversion time = 13 * 8us = 104us
*/

/** ADMUX – ADC Multiplexer Selection Register p.281
|7      |6      |5      |4      |3      |2      |1      |0      |
|REFS1  |REFS0  |ADLAR  |MUX4   |MUX3   |MUX2   |MUX1   |MUX0   |

REFS1:0: Reference Selection Bits
Auswahl der Referenzspannung.
Achtung: Die Interne referenz wird möglicherweise nicht benutzt, falls eine Referenzspannung an Pin AREF anliegt.
Table 26-3 p.281
REFS1 = 0, REFS0 = 0 --> Refernzspannung an Pin AREF
REFS1 = 0, REFS0 = 1 --> Refernzspannung = AVCC (Power für ADC), Kondensator an Pin AREF (stabilisierung)
Änderungen werden erst nach laufender Conversation aktiv!

ADLAR: ADC Left Adjust Result
Wenn = 1, Resultat in ADC Data Register ist links ausgerichtet
Wenn = 0, Resultat in ADC Data Register ist rechts ausgerichtet

MUX4:0: Analog Channel and Gain Selection Bits
Auswahl der analogen Eingänge, die mit dem ADC verbunden sind
Für die Werte siehe Table 26-4 p.282*/
#define ADC_MULTIPLEXER ADMUX
#define ADC_MULTIPLEXER_value ((0<<REFS1) | (1<<REFS0) | (1<<ADLAR) | (0<<MUX4) | (0<<MUX3) | (0<<MUX2) | (0<<MUX1) | (0<<MUX0))

#define ADC_SELECT_RESET        (ADC_MULTIPLEXER &= ~((1<<MUX4)|(1<<MUX3)|(1<<MUX2)|(1<<MUX1)|(1<<MUX0)));\
                                (ADC_CONTROL_B &= ~(1<<MUX5));

#define ADC_SELECT_CURRENT_S01  ADC_SELECT_RESET;\
                                (ADC_MULTIPLEXER |= ((1<<MUX1) | (1<<MUX0)));\
                                (ADC_CONTROL_B |= (1<<MUX5)); // ADC11 -> 100011

#define ADC_SELECT_CURRENT_S02  ADC_SELECT_RESET;\
                                (ADC_MULTIPLEXER |= (1<<MUX1));\
                                (ADC_CONTROL_B |= (1<<MUX5)); // ADC10 -> 100010

#define ADC_SELECT_PHASE_A      ADC_SELECT_RESET;\
                                (ADC_MULTIPLEXER |= ((1<<MUX2) | (1<<MUX0)));\
                                (ADC_CONTROL_B |= (1<<MUX5)); // ADC13 -> 100101

#define ADC_SELECT_PHASE_B      ADC_SELECT_RESET;\
                                (ADC_MULTIPLEXER |= ((1<<MUX2)|(1<<MUX1)));\
                                (ADC_CONTROL_B |= (1<<MUX5)); // ADC14 -> 100110

#define ADC_SELECT_VOLTAGE_C    ADC_SELECT_RESET;\
                                (ADC_MULTIPLEXER |= ((1<<MUX2)|(1<<MUX1)|(1<<MUX0)));\
                                (ADC_CONTROL_B |= (1<<MUX5)); // ADC15 -> 100111

#define ADC_SELECT_BATTERY      ADC_SELECT_RESET;\
                                (ADC_MULTIPLEXER |= (1<<MUX2));\
                                (ADC_CONTROL_B |= (1<<MUX5)) // ADC12 -> 100100

#define ADC_SELECT_HALL_TAIL    ADC_SELECT_RESET;\
                                (ADC_MULTIPLEXER |= (1<<MUX0));\
                                (ADC_CONTROL_B |= (1<<MUX5)); // ADC9 -> 100001

#define ADC_SELECT_HALL_NOSE    ADC_SELECT_RESET;\
                                (ADC_CONTROL_B |= (1<<MUX5)); // ADC8 -> 100000


#define ADC_SELECT_REF1         ADC_SELECT_RESET // ADC0 -> 000000

#define ADC_SELECT_REF2         ADC_SELECT_RESET;\
                                (ADC_MULTIPLEXER |= (1<<MUX0)) // ADC1 -> 000001

#define ADC_SELECT_REF3         ADC_SELECT_RESET;\
                                (ADC_MULTIPLEXER |= (1<<MUX1))// ADC2 -> 000010

#define ADC_SELECT_REF4         ADC_SELECT_RESET;\
                                (ADC_MULTIPLEXER |= ((1<<MUX1)|(1<<MUX0)))// ADC3 -> 000011

/** ADCSRB – ADC Control and Status Register B p.282
|7      |6      |5      |4      |3      |2      |1      |0      |
|-      |ACME   |-      |-      |MUX5   |ADTS2  |ADTS1  |ADTS0  |

MUX5: Analog Channel and Gain Selection Bit
6. Bit ergänzend zu MUX4:0 in ADMUX

ADTS2:0: ADC Auto Trigger Source
Quelle für Auto-Trigger wählen.
Wenn ADATE (in ADCSRA) = 0, haben Einstellungen keinen Effekt.
Free Running Mode wird hier eingestellt (Trigger = Interrupt von erfolgreicher ADC Wandlung)*/
#define ADC_CONTROL_B ADCSRB

/** ADCSRA – ADC Control and Status Register A
|7      |6      |5      |4      |3      |2      |1      |0      |
|ADEN   |ADSC   |ADATE  |ADIF   |ADIE   |ADPS2  |ADPS1  |ADPS0  |

ADEN: ADC Enable
Wenn = 1, der ADC wird aktiviert
Wenn = 0, der ADC wird deaktiviert, jedoch erst nach der laufenden konversion

ADSC: ADC Start Conversion
Single conversion mode:
Auf 1 setzen um konversation zu starten.
Bleibt so lange auf 1 bis Wandlung beendet (dann auf 0)
Free Running mode: Auf 1 setzen um erste Wandlung zu starten.

ADATE: ADC Auto Trigger Enable
Wenn = 1, wird das Auto Triggering des ADC erlaubt

ADIF: ADC Interrupt Flag
Gesetzt, wenn Wandlung fertig und Datenregister aktualisiert wurde. Kann Interrupt auslösen.
Wird von Hardware zurückgesetzt wenn Interrupt behandelt wird. Sonst manuel zurücksetzen.

ADIE: ADC Interrupt Enable
Interrupt für ADC erlauben

ADPS2:0: ADC Prescaler Select Bits
Table 26-5 p.285
ADPS2=1 ADPS1=1 ADPS0=0 : clk/64
ADPS2=1 ADPS1=1 ADPS0=1 : clk/128
*/
#define ADC_CONTROL_A ADCSRA
#define ADC_CONTROL_A_value ((1<<ADEN) | (0<<ADSC) | (0<<ADATE) | (0<<ADIF) | (1<<ADIE) | (1<<ADPS2) | (1<<ADPS1) | (0<<ADPS0))

#define ADC_START (ADC_CONTROL_A |= (1<<ADSC))
#define ADC_IS_FINISHED (ADC_CONTROL_A & (1<<ADIF))
#define ADC_RESET_FINISHED_FLAG (ADC_CONTROL_A &= ~(1<<ADIF))

/** ADCL and ADCH – The ADC Data Register
rechts ausgerichtet
|7      |6      |5      |4      |3      |2      |1      |0      |
|-      |-      |-      |-      |-      |-      |ADC9   |ADC8   | --> ADCH
|ADC7   |ADC6   |ADC5   |ADC4   |ADC3   |ADC2   |ADC1   |ADC0   | --> ADCL

LINKS ausgerichtet
|7      |6      |5      |4      |3      |2      |1      |0      |
|ADC9   |ADC8   |ADC7   |ADC6   |ADC5   |ADC4   |ADC3   |ADC2   | --> ADCH
|ADC1   |ADC0   |-      |-      |-      |-      |-      |-      | --> ADCL

Resultat der Wandlung. Falls Differentiel gemessen wurd in Zweierkomplement dargestellt.
Update der Daten erst nachdem ADCH gelesen wurde (zuwerst low lesen, dann high)
Falls nur 8bit Auflösung: Links ausgerichtete Daten. Es reict blos das High Byte zu lesen.
*/
#define ADC_DATA ADCH

/**DIDR0 – Digital Input Disable Register 0
|7      |6      |5      |4      |3      |2      |1      |0      |
|ADC7D  |ADC6D  |ADC5D  |ADC4D  |ADC3D  |ADC2D  |ADC1D  |ADC0D  |

ADC7D:ADC0D: ADC7:0 Digital Input Disable
Wenn = 1, digitaler Buffer zu ADC Pin (ADC7:0) disabled*/
#define ADC_0TO7_DIGITAL_IO_DISABLE_REGISTER DIDR0
#define ADC_0TO7_DIGITAL_IO_DISABLE_REGISTER_value ((0<<ADC7D) | (0<<ADC6D) | (0<<ADC5D) | (0<<ADC4D) | (1<<ADC3D) | (1<<ADC2D) | (1<<ADC1D) | (1<<ADC0D))


/**DIDR2 – Digital Input Disable Register 2
|7      |6      |5      |4      |3      |2      |1      |0      |
|ADC15D |ADC14D |ADC13D |ADC12D |ADC11D |ADC10D |ADC9D  |ADC8D  |

ADC15D:ADC8D: ADC15:8 Digital Input Disable
Wenn = 1, digitaler Buffer zu ADC Pin (ADC15:8) disabled*/
#define ADC_8TO15_DIGITAL_IO_DISABLE_REGISTER DIDR2
#define ADC_8TO15_DIGITAL_IO_DISABLE_REGISTER_value ((1<<ADC15D) | (1<<ADC14D) | (1<<ADC13D) | (1<<ADC12D) | (1<<ADC11D) | (1<<ADC10D) | (1<<ADC9D) | (1<<ADC8D))

// Interrupts vectors
#define CONVERSION_COMPLETE_ISR ADC_vect

// constantes
#define PHASE_CURRENT_OFFSET 128

// includes
#include "system.h"
#include "logger.h"

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

// variables
typedef enum {
            no_conv,
            current_s01_conv,
            current_s02_conv,
            hall_sensor_nose_conv,
            hall_sensor_tail_conv,
            battery_conv,
            reference1_conv,
            reference2_conv,
            reference3_conv,
            reference4_conv
            } State;
static State state = no_conv;

int8_t lastS01Current = 0;
int8_t lastS02Current = 0;

uint8_t lastHallSensorNose = 0;
uint8_t lastHallSensorTail = 0;

uint8_t lastBattery = 0;

uint8_t lastReference1 = 0;
uint8_t lastReference2 = 0;
uint8_t lastReference3 = 0;
uint8_t lastReference4 = 0;

static void (*measurementDataAvailableListener)(char phaseLastCurrentMeassure);
static char phaseToMeasure;

// functions
void proceedNextMeasure();

void initAnalog()
{
    logMsgLn("Init Analog...");

    ADC_MULTIPLEXER |= ADC_MULTIPLEXER_value;
    ADC_CONTROL_A |= ADC_CONTROL_A_value;
    ADC_0TO7_DIGITAL_IO_DISABLE_REGISTER |= ADC_0TO7_DIGITAL_IO_DISABLE_REGISTER_value;
    ADC_8TO15_DIGITAL_IO_DISABLE_REGISTER |= ADC_8TO15_DIGITAL_IO_DISABLE_REGISTER_value;

    startMeasureProcedure(0); // init modules (first conversion takes longer)
}

void registerMeasurementDataAvailableListener(void (*listener)(char phaseLastCurrentMeassure))
{
    measurementDataAvailableListener = listener;
}

int8_t startMeasureProcedure(char newPhaseToMeasure)
{
    if(state == no_conv)
    {
        phaseToMeasure = newPhaseToMeasure;
        proceedNextMeasure();
        return 0;
    }

    return -1;
}

void proceedNextMeasure()
{
    switch(state)
    {
        case no_conv:
            // select & start next measure
            switch(phaseToMeasure)
            {
                case 'A':
                state = current_s01_conv;
                ADC_SELECT_CURRENT_S01;
                break;

                case 'B':
                state = current_s02_conv;
                ADC_SELECT_CURRENT_S02;
                break;

                default:
                state = hall_sensor_nose_conv;
                lastS01Current = 0;
                lastS01Current = 0;
                ADC_SELECT_HALL_NOSE;
                break;
            }

            ADC_START;
        break;

        case hall_sensor_nose_conv:
            // store last data
            lastHallSensorNose = ADC_DATA;

            // select & start next measure
            state = hall_sensor_tail_conv;
            ADC_SELECT_HALL_TAIL;
            ADC_START;
        break;

        case hall_sensor_tail_conv:
            // store last data
            lastHallSensorTail = ADC_DATA;

            // select & start next measure
            state = battery_conv;
            ADC_SELECT_BATTERY;
            ADC_START;
        break;

        case battery_conv:
            // store last data
            lastBattery = ADC_DATA;

            // select & start next measure
            state = no_conv;

            if(measurementDataAvailableListener != 0)
            {
                measurementDataAvailableListener(phaseToMeasure);
            }
        break;

        case current_s01_conv:
            // store last data
            lastS01Current = (128 - ADC_DATA);

            // select & start next measure
            state = hall_sensor_nose_conv;
            ADC_SELECT_HALL_NOSE;
            ADC_START;
        break;

        case current_s02_conv:
            // store last data
            lastS02Current = (128 - ADC_DATA);

            // select & start next measure
            state = hall_sensor_nose_conv;
            ADC_SELECT_HALL_NOSE;
            ADC_START;
        break;

        default:
        // do nothing
        break;
    }
}

ISR (CONVERSION_COMPLETE_ISR)
{
    proceedNextMeasure();
}

uint8_t getLastHallSensorNoseVoltage()
{
    return lastHallSensorNose;
}

uint8_t getLastHallSensorTailVoltage()
{
    return lastHallSensorTail;
}

int8_t getLastPhaseACurrent()
{
    return lastS01Current;
}

int8_t getLastPhaseBCurrent()
{
    return lastS02Current;
}

uint8_t getLastBattery()
{
    return lastBattery;
}

// sensor 0 = Front
// sensor 1 = Back
uint8_t readInterfaceSensorsVoltage(char sensor)
{
    switch(sensor)
    {
        case 0:
        return lastHallSensorNose;

        case 1:
        return lastHallSensorTail;

        default:
        return 0;
    }
}

// blocking functions
// sensor 0 = Front
// sensor 1 = Back
void readInterfaceSensorsVoltageBLOCKING(char sensor)
{
    switch(sensor)
    {
        case 0:
        ADC_SELECT_HALL_NOSE;
        break;

        case 1:
        ADC_SELECT_HALL_TAIL;
        break;
    }

    ADC_START;

    while(!ADC_IS_FINISHED)
    {}
    ADC_RESET_FINISHED_FLAG;

    switch(sensor)
    {
        case 0:
        lastHallSensorNose = ADC_DATA;
        break;

        case 1:
        lastHallSensorTail = ADC_DATA;
        break;
    }
}

char readReference1BLOCKING(void)
{
    ADC_SELECT_REF1;
    ADC_START;

    while(!ADC_IS_FINISHED)
    {}
    ADC_RESET_FINISHED_FLAG;

    return ADC_DATA;
}

char readReference2BLOCKING(void)
{
    ADC_SELECT_REF2;
    ADC_START;

    while(!ADC_IS_FINISHED)
    {}
    ADC_RESET_FINISHED_FLAG;

    return ADC_DATA;
}

char readReference3BLOCKING(void)
{
    ADC_SELECT_REF3;
    ADC_START;

    while(!ADC_IS_FINISHED)
    {}
    ADC_RESET_FINISHED_FLAG;

    return ADC_DATA;
}

char readReference4BLOCKING(void)
{
    ADC_SELECT_REF4;
    ADC_START;

    while(!ADC_IS_FINISHED)
    {}
    ADC_RESET_FINISHED_FLAG;

    return ADC_DATA;
}
