/*
 * logger.c
 *
 * Created: 20.04.2017
 * Author : Simon Bühlmann
 */

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "ringbuffer.h"
#include "system.h"

void writeNewLine()
{
    transmitChar('\r'); // return
    transmitChar('\n'); // new line
}

void logMsgInBuffer(char *msg)
{
    bufferIn((uint16_t)msg); //store address of string
}

void writeBuffered()
{
    volatile unsigned char logsAvailable = 1;

    while(logsAvailable)
    {
        uint16_t* pToAdressMsg;

        logsAvailable = bufferOut(pToAdressMsg); //get address of string

        if(logsAvailable)
        {
            transmitString((char*)*pToAdressMsg);
            writeNewLine();
        }
        else
        {
            return;
        }
    }
}

void logMsg(char* msg)
{
    transmitString(msg);
    writeNewLine();
}

void logVar(char *name, int var, char stringLenght)
{
    char str[stringLenght];
    itoa(var, str, 10);
    transmitString(name);
    transmitString(": ");
    transmitString(str);
    writeNewLine();
}
