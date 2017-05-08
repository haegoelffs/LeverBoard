// ___EXTERNAL COMPERATORS___

#include "system.h"
#include "logger.h"

#include <avr/io.h>
#include <avr/interrupt.h>

/** TEICRA – External Interrupt Control Register A p.110
    |7      |6      |5      |4      |3      |2      |1      |0      |
    |ISC31  |ISC30  |ISC21  |ISC20  |ISC11  |ISC10  |ISC01  |ISC00  |

   ISC31, ISC30 – ISC00, ISC00: External Interrupt 3 - 0 Sense Control Bits
   Interrupt festlegen für Pins INT0 - INT3
   Table 15-1 p.110
   ISCn1 = 0, ISCn0 = 1 --> Fallende und steigende Flanken erzeugen Interrupt*/
#define EXT_INTERRUPT_0TO3_CONTROLL_REGISTER TEICRA

/** EICRB – External Interrupt Control Register B p.111
    |7      |6      |5      |4      |3      |2      |1      |0      |
    |ISC71  |ISC70  |ISC61  |ISC60  |ISC51  |ISC50  |ISC41  |ISC40  |

   ISC71, ISC70 - ISC41, ISC40: External Interrupt 7 - 4 Sense Control Bits
   Interrupt festlegen für Pins INT7 - INT4
   Table 15-3 p.111
   ISCn1 = 0, ISCn0 = 1 --> Fallende und steigende Flanken erzeugen Interrupt*/
#define EXT_INTERRUPT_4TO7_CONTROLL_REGISTER EICRB

/** EIMSK – External Interrupt Mask Register p.111
    |7      |6      |5      |4      |3      |2      |1      |0      |
    |INT7   |INT6   |INT5   |INT4   |INT3   |INT2   |INT1   |INT0   |
   erlauben der externen Interrupts*/
#define ENABLE_EXTERN_INTERRUPTS_REGISTER EIMSK
#define INT_COMP_A 4
#define INT_COMP_B 5
#define INT_COMP_C 6

/** EIFR – External Interrupt Flag Register p.112
    |7      |6      |5      |4      |3      |2      |1      |0      |
    |INTF7  |INTF6  |INTF5  |INTF4  |INTF3  |INTF2  |INTF1  |INTF0  |
   Wenn Interrupt erfolgt, wird entsprechendes Flag auf 1 gesetzt.
   Nach Interruptrountine durch HW zurückgesetzt.*/

/** Interrupt vectors
*/
#define PHASE_A_ISR INT4_vect
#define PHASE_B_ISR INT5_vect
#define PHASE_C_ISR INT6_vect

/** GPIOs
*/
#define TRISTATE_INPUT 0
#define TRISTATE_OUTPUT 1
#define TRISTATE_COMP DDRE
#define PORT_COMP PORTE


static void (*listenerPhaseA)(char edge);
static void (*listenerPhaseB)(char edge);
static void (*listenerPhaseC)(char edge);

void initComp()
{
    logMsg("Init comperators...");

    TRISTATE_COMP |= ((TRISTATE_INPUT<<INT_COMP_A) |(TRISTATE_INPUT<<INT_COMP_B) |(TRISTATE_INPUT<<INT_COMP_C)); // config as inputs

    EXT_INTERRUPT_4TO7_CONTROLL_REGISTER |= (1<<0);
    EXT_INTERRUPT_4TO7_CONTROLL_REGISTER &= ~(1<<(0+1));

    EXT_INTERRUPT_4TO7_CONTROLL_REGISTER |= (1<<2);
    EXT_INTERRUPT_4TO7_CONTROLL_REGISTER &= ~(1<<(2+1));

    EXT_INTERRUPT_4TO7_CONTROLL_REGISTER |= (1<<4);
    EXT_INTERRUPT_4TO7_CONTROLL_REGISTER &= ~(1<<(4+1));

    ENABLE_EXTERN_INTERRUPTS_REGISTER |= (1<<INT_COMP_A);
}

void setEnableCompA(char enable)
{
    if(enable)
    {
        ENABLE_EXTERN_INTERRUPTS_REGISTER |= (1<<INT_COMP_A);
    }
    else
    {
        ENABLE_EXTERN_INTERRUPTS_REGISTER &= ~(0<<INT_COMP_A);
    }
}

void setEnableCompB(char enable)
{
    if(enable)
    {
        ENABLE_EXTERN_INTERRUPTS_REGISTER |= (1<<INT_COMP_B);
    }
    else
    {
        ENABLE_EXTERN_INTERRUPTS_REGISTER &= ~(0<<INT_COMP_B);
    }
}

void setEnableCompC(char enable)
{
    if(enable)
    {
        ENABLE_EXTERN_INTERRUPTS_REGISTER |= (1<<INT_COMP_C);
    }
    else
    {
        ENABLE_EXTERN_INTERRUPTS_REGISTER &= ~(0<<INT_COMP_C);
    }
}

void registerVoltageZeroCrossingListenerPhaseA(void (*listener)(char edge))
{
    logMsg("Register zero crossing listener phase A");
    listenerPhaseA = listener;
}

void registerVoltageZeroCrossingListenerPhaseB(void (*listener)(char edge))
{
    logMsg("Register zero crossing listener phase B");
    listenerPhaseB = listener;
}

void registerVoltageZeroCrossingListenerPhaseC(void (*listener)(char edge))
{
    logMsg("Register zero crossing listener phase C");
    listenerPhaseC = listener;
}

ISR (PHASE_A_ISR)
{
    setEnableCompA(0); // disable isr -> interrupt only on first zero crossing

    if(listenerPhaseA != 0)
    {
        listenerPhaseA((PORT_COMP >> INT_COMP_A) & 1);
    }
}

ISR (PHASE_B_ISR)
{
    setEnableCompB(0); // disable isr -> interrupt only on first zero crossing

    if(listenerPhaseB != 0)
    {
        listenerPhaseB((PORT_COMP >> INT_COMP_B) & 1);
    }
}

ISR (PHASE_C_ISR)
{
    setEnableCompC(0); // disable isr -> interrupt only on first zero crossing

    // phase C
    if(listenerPhaseC != 0)
    {
        listenerPhaseC((PORT_COMP >> INT_COMP_C) & 1);
    }
}
