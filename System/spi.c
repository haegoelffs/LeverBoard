// __SPI________________________________________________________
// Registers:
// SPCR – SPI Control Register
//  |7      |6      |5      |4      |3      |2      |1      |0      |
//  |SPIE   |SPE    |DORD   |MASTR  |CPOL   |CPHA   |SPR1   |SPR0   |
#define SPI_CONTROL_REGISTER

// SPIE: SPI Interrupt Enable
#define SPIE_value 1

// SPE: SPI Enable
#define SPE_value 1

// DORD: Data Order
// Wenn = 1, LSB zuerst
// Wenn = 0, MSB zuerst <--|
#define DORD_value 0

// MSTR: Master/Slave Select
// Wenn = 1, --> Mastermode <--|
// Wenn = 0, --> Slavemode
// Falls !SS als Eingang konfiguriert & = 0 während MSTR gesetzt ist, wird MSTR = 0
#define MSTR_value 1

// CPOL: Clock Polarity
// Wenn = 1, SCK high wenn untätig
// Wenn = 0, SCK low wenn untätig<--|
#define CPOL_value 0

// CPHA: Clock Phase
// Wenn = 1, sampling findet bei hinterkante von clk statt
// Wenn = 0, sampling findet bei vorderkante von clk statt<--|
#define CPHA_value 0

// SPR1, SPR0: SPI Clock Rate Select 1 and 0
// Table 21-5 p.198
// SPR1 = 0, SPR0 = 0 --> f_osc/4
// Berechnung: Min. T_clock = 100ns (p.10 Datasheet DRV8301)
// 1/100ns = 10MHz --> Max. Frequenz
#define SPR1_value 0
#define SPR0_value 0


// SPSR – SPI Status Register
//  |7      |6      |5      |4      |3      |2      |1      |0      |
//  |SPIF   |WCOL   |-      |-      |-      |-      |-      |SPI2X  |
#define SPI_STATUS_REGISTER SPSR

// SPIF: SPI Interrupt Flag
// Gesetzt wenn:
// - serieller Transfer beendet
// - wenn Mastermode, !SS ein Eingang & aus 0 gezogen
#define SPIF_value 1

// WCOL: Write COLlision Flag
// gesetzt, falls Datenregister SPDR beschrieben wird während Datentransfer
// zurückgesetzt, wenn SPCR gelesen und Datebregister neu beschrieben wird

// SPI2X: Double SPI Speed Bit
// Wenn gesetzt, wird die SCK Frequenz verdoppelt
#define SPI2X_value 0

// SPDR – SPI Data Register
//  |7      |6      |5      |4      |3      |2      |1      |0      |
//  |MSB    |       |       |       |       |       |       |LSB    |
#define SPI_DATA SPDR

// Bridge Driver:
#define ADDRESS_STATUS_REGISTER_1 0b0000
#define ADDRESS_STATUS_REGISTER_2 0b0001
#define ADDRESS_CONTROL_REGISTER_1 0b0010
#define ADDRESS_CONTROL_REGISTER_2 0b0011



void initSPI()
{
    SPI_CONTROL_REGISTER |= ((SPIE_value << SPIE) | (SPE_value << SPE) | (DORD_value << DORD) | (CPOL_value << CPOL) | (SPR1_value << SPR1) | (SPR0_value << SPR0));
    SPI_STATUS_REGISTER |= ((SPIF_value<<SPIF) | (SPI2X_value<<SPI2X));
}

void SPI_MasterTransmit(char cData)
{
    /* Start transmission */
    SPI_DATA = cData;
    /* Wait for transmission complete */
    while(!(SPSR & (1<<SPIF)));
}
char SPI_SlaveReceive(void)
{
    /* Wait for reception complete */
    while(!(SPSR & (1<<SPIF)));
    /* Return Data Register */
    return SPDR;
}

ISR (TIMER0_COMPA_vect)
{

}


