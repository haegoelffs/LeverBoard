#ifndef LOGGER_H_INCLUDED
#define LOGGER_H_INCLUDED

#define LOG_ERROR
#define LOG_INFO

#include "ringbufferDriveData.h"
extern BufferDriveData *pDataBuffer;

void logMsg(char *msg);
void logMsgLn(char *msg);
void logVar(char *name, int var, char stringLenght);
void logNamedUnsignedInt(char *name, uint16_t var, char stringLenght);
void logUnsignedInt(uint16_t var, char stringLenght);
void logSignedInt(int16_t var, char stringLenght);
void logNamedSignedInt(char *name, int16_t var, char stringLenght);

void writeNewLine();
#endif // LOGGER_H_INCLUDED
