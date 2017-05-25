#include "system.h"
#include "logger.h"
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>


// Registers:
/** SPCR – SPI Control Register
|7      |6      |5      |4      |3      |2      |1      |0      |
|SPIE   |SPE    |DORD   |MASTR  |CPOL   |CPHA   |SPR1   |SPR0   |

SPIE: SPI Interrupt Enable

SPE: SPI Enable

DORD: Data Order
Wenn = 1, LSB zuerst
Wenn = 0, MSB zuerst <--|

MSTR: Master/Slave Select
Wenn = 1, --> Mastermode <--|
Wenn = 0, --> Slavemode
Falls !SS als Eingang konfiguriert & = 0 während MSTR gesetzt ist, wird MSTR = 0

CPOL: Clock Polarity
Wenn = 1, SCK high wenn untätig
Wenn = 0, SCK low wenn untätig<--|

CPHA: Clock Phase
Wenn = 1, sampling findet bei hinterkante von clk statt
Wenn = 0, sampling findet bei vorderkante von clk statt<--|

SPR1, SPR0: SPI Clock Rate Select 1 and 0
Table 21-5 p.198
SPR1 = 0, SPR0 = 0 --> f_osc/4
Berechnung: Min. T_clock = 100ns (p.10 Datasheet DRV8301)
1/100ns = 10MHz --> Max. Frequenz**/
#define SPI_CONTROL_REGISTER SPCR

#define SPIE_value 1
#define SPE_value 1
#define DORD_value 0
#define MSTR_value 1
#define CPOL_value 0
#define CPHA_value 0

#define SPR1_value 0
#define SPR0_value 0

/** SPSR – SPI Status Register
|7      |6      |5      |4      |3      |2      |1      |0      |
|SPIF   |WCOL   |-      |-      |-      |-      |-      |SPI2X  |

SPIF: SPI Interrupt Flag
Gesetzt wenn:
- serieller Transfer beendet
- wenn Mastermode, !SS ein Eingang & aus 0 gezogen

WCOL: Write COLlision Flag
gesetzt, falls Datenregister SPDR beschrieben wird während Datentransfer
zurückgesetzt, wenn SPCR gelesen und Datebregister neu beschrieben wird

SPI2X: Double SPI Speed Bit
Wenn gesetzt, wird die SCK Frequenz verdoppelt**/
#define SPI_STATUS_REGISTER SPSR

#define SPI_DATA_TRANSFER_FINISHED (SPI_STATUS_REGISTER & (1<<SPIF))

/**SPDR – SPI Data Register
|7      |6      |5      |4      |3      |2      |1      |0      |
|MSB    |       |       |       |       |       |       |LSB    |**/
#define SPI_DATA SPDR

// gpio
#define TRISTATE_INPUT 0
#define TRISTATE_OUTPUT 1

#define PORT_SPI PORTB
#define TRISTATE_SPI DDRB
#define PIN_CS_BRIDGE_DRIVER 0

#define SELECT_BRIDGE_DRIVER (PORT_SPI &= ~(1<<PIN_CS_BRIDGE_DRIVER)) // active low!
#define UNSELECT_BRIDGE_DRIVER (PORT_SPI |= (1<<PIN_CS_BRIDGE_DRIVER))

// Bridge Driver:
#define ADDRESS_STATUS_REGISTER_1 0b0000
#define ADDRESS_STATUS_REGISTER_2 0b0001
#define ADDRESS_CONTROL_REGISTER_1 0b0010
#define ADDRESS_CONTROL_REGISTER_2 0b0011

/** Bridge Input Data
|15     |14     |13     |12     |11     |10     |9      |8      |7      |6      |5      |4      |3      |2      |1      |0      |
|R/W    |ADR3   |ADR2   |ADR1   |ADR0   |DATA10 |DATA9  |DATA8  |DATA6  |DATA5  |DATA4  |DATA3  |DATA2  |DATA2  |DATA1  |DATA0  |
**/
#define WRITE_COMMAND 0
#define READ_COMMAND 1

#define READ_STATUS_REGISTER_1_COMMAND_high ((READ_COMMAND<<7) | (ADDRESS_STATUS_REGISTER_1<<3))
#define READ_STATUS_REGISTER_2_COMMAND_high ((READ_COMMAND<<7) | (ADDRESS_STATUS_REGISTER_2<<3))

// Status register 1 data p.22
#define FAULT 10 // frame error in previous cycle
#define GVDD_UV 9 // gate drive supply undervoltage
#define PVDD_UV 8 // undervoltage protection
#define OTSD 7 // overtemp shutdown
#define OTW 6 // overtemp warning
#define FETHA_OC 5
#define FETLA_OC 4
#define FETHB_OC 3
#define FETLB_OC 2
#define FETHC_OC 1
#define FETLC_OC 0

// Status register 2 data p.22
#define GVDD_OV 7 // frame error in previous cycle

// interrupt
#define SPI_SERIAL_TRANSFER_COMPLETE_vector SPI_STC_vect

uint16_t lastStatusRegister1Value = 0;
uint16_t lastStatusRegister2Value = 0;

void initSPI()
{
    logMsgLn("Init SPI...");

    SPI_CONTROL_REGISTER |= ((SPIE_value << SPIE) | (SPE_value << SPE) | (DORD_value << DORD) | (CPOL_value << CPOL) | (SPR1_value << SPR1) | (SPR0_value << SPR0));

    TRISTATE_SPI |= (TRISTATE_OUTPUT<<PIN_CS_BRIDGE_DRIVER);
    UNSELECT_BRIDGE_DRIVER;
}

void spi_readStatusRegisters_BLOCKING()
{
    SELECT_BRIDGE_DRIVER;
    // write read status register 1 command
    // highbyte
    SPI_DATA = READ_STATUS_REGISTER_1_COMMAND_high;
    while(!SPI_DATA_TRANSFER_FINISHED);

    // lowbyte
    SPI_DATA = 0;
    while(!SPI_DATA_TRANSFER_FINISHED);

    UNSELECT_BRIDGE_DRIVER;
    _delay_ms(5);
    SELECT_BRIDGE_DRIVER;

    // write read status register 2 command and receive data from previous cycle (status register 1 data)
    // highbyte
    SPI_DATA = READ_STATUS_REGISTER_2_COMMAND_high;
    while(!SPI_DATA_TRANSFER_FINISHED);
    lastStatusRegister1Value = (SPI_DATA<<8);

    // lowbyte
    SPI_DATA = 0;
    while(!SPI_DATA_TRANSFER_FINISHED);
    lastStatusRegister1Value = SPI_DATA;

    UNSELECT_BRIDGE_DRIVER;
    _delay_ms(5);
    SELECT_BRIDGE_DRIVER;

    // receive data from previous cycle (status register 2 data)
    SPI_DATA = 0;
    while(!SPI_DATA_TRANSFER_FINISHED);
    lastStatusRegister2Value = (SPI_DATA<<8);

    // lowbyte
    SPI_DATA = 0;
    while(!SPI_DATA_TRANSFER_FINISHED);
    lastStatusRegister2Value = SPI_DATA;
}

uint16_t getLastStatusRegister1Value()
{
    return lastStatusRegister1Value;
}

uint16_t getLastStatusRegister2Value()
{
    return lastStatusRegister2Value;
}

ISR (SPI_SERIAL_TRANSFER_COMPLETE_vector)
{

}


