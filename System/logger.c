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
#include <stdio.h>


#include "system.h"

void writeNewLine()
{
    transmitChar('\r'); // return
    transmitChar('\n'); // new line
}

void logMsg(char* msg)
{
    transmitString(msg);
}

void logMsgLn(char *msg)
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
}

void logNamedUnsignedInt(char *name, uint16_t var, char stringLenght)
{
    char str[stringLenght];
    sprintf(str, "%s:%u", name, var);
    transmitString(str);
}

void logUnsignedInt(uint16_t var, char stringLenght)
{
    char str[stringLenght];
    sprintf(str, "%u", var);
    transmitString(str);
}

void logSignedInt(int16_t var, char stringLenght)
{
    char str[stringLenght];
    sprintf(str, "%i", var);
    transmitString(str);
}

void logSignedIntLine(int16_t var1, int16_t var2, int16_t var3, int16_t var4, char stringLenght)
{
    char str[stringLenght];
    sprintf(str, "%i;%i;%i;%i", var1, var2, var3, var4);
    transmitString(str);
}

void logNamedSignedInt(char *name, int16_t var, char stringLenght)
{
    char str[stringLenght];
    sprintf(str, "%s:%i", name, var);
    transmitString(str);
}
