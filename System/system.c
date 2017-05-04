#include "system.h"
#include "leverboardHardware.h"
#include "logger.h"

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

// uart
void initUART()
{
    UART_REGISTER_A |= UART_REGISTER_A_VALUE;
    UART_REGISTER_B |= UART_REGISTER_B_VALUE;
    UART_REGISTER_C |= UART_REGISTER_C_VALUE;

    UART_BAUD_RATE_HIGH_BYTE = UART_BAUD_RATE_HIGH_BYTE_VALUE;
    UART_BAUD_RATE_LOW_BYTE = UART_BAUD_RATE_LOW_BYTE_VALUE;
}

// Source: https://www.mikrocontroller.net/articles/AVR-GCC-Tutorial/Der_UART
void transmitString(char *msg)
{
    while (*msg) // point on first char
    {
        // msg is zero terminated -> loop will stop at the end of the string
        transmitChar(*msg);
        msg++; // increase pointer to next char
    }
}

// Source: Atmel ATmega640/V-1280/V-1281/V-2560/V-2561/V Datasheet p.207
void transmitChar(char data)
{
    while(!(UART_REGISTER_A & (1<<UART_DATA_REG_EMPTY_BIT)));

    UART_SEND_DATA = data;
}
