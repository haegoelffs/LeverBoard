#ifndef LOGGER_H_INCLUDED
#define LOGGER_H_INCLUDED

// ISR functions
void logMsgInBuffer(char *msg);
void logVar(char *name, int var, char stringLenght);
void writeBuffered();

// normal functions
void logMsg(char *msg);
#endif // LOGGER_H_INCLUDED
