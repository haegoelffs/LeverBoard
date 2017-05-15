#include <stdint.h>
#include <stdlib.h>

#include "ringbuffer.h"
#include "system.h"
#include "loggerISR.h"

void logMsgBuffered(char *msg)
{
    if(!bufferIn((uint16_t)msg)) //store address of string
    {
        logMsg("Buffer isr logger fail");
    }
    //logMsgBuffered("\r\n");
}

void logUnsignedIntBuffered(uint16_t var, char stringLenght)
{
    char str[stringLenght];
    sprintf(str, "%u\r\n", var);

    if(!bufferIn((uint16_t)str)) //store address of string
    {
        logMsg("Buffer isr logger fail");
    }
}

void logNamedUnsignedIntBuffered(char *name, uint16_t var, char stringLenght)
{
    char str[stringLenght];
    sprintf(str, "%s:%u\r\n", name, var);

    if(!bufferIn((uint16_t)str)) //store address of string
    {
        logMsg("Buffer isr logger fail");
    }
}

void logSignedIntBuffered(int16_t var, char stringLenght)
{
    char str[stringLenght];
    sprintf(str, "%i\r\n", var);

    if(!bufferIn((uint16_t)str)) //store address of string
    {
        logMsg("Buffer isr logger fail");
    }
}

void logNamedSignedIntBuffered(char *name, uint16_t var, char stringLenght)
{
    char str[stringLenght];
    sprintf(str, "%s:%i\r\n", name, var);

    if(!bufferIn((uint16_t)str)) //store address of string
    {
        logMsg("Buffer isr logger fail");
    }
}

void logSignedIntLineBuffered(int16_t var1, int16_t var2, int16_t var3, int16_t var4, char stringLenght)
{
    char str[stringLenght];
    sprintf(str, "%i;%i;%i;%i\r\n", var1, var2, var3, var4);

    if(!bufferIn((uint16_t)str)) //store address of string
    {
        logMsg("Buffer isr logger fail");
    }
}

void writeBuffered()
{
    unsigned char logsAvailable = 1;

    while(logsAvailable)
    {
        uint16_t adressMsg;
        uint16_t *pToAdressMsg = &adressMsg;

        logsAvailable = bufferOut(pToAdressMsg); //get address of string

        if(logsAvailable)
        {
            char *temp;
            temp = *pToAdressMsg;

            transmitString(temp);
        }
        else
        {
            return;
        }
    }
}
