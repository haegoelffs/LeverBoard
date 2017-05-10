#ifndef LOGGER_H_INCLUDED
#define LOGGER_H_INCLUDED

// ISR functions
void logMsgInBuffer(char *msg);
void writeBuffered();

// normal functions
void logMsg(char *msg);
void logVar(char *name, int var, char stringLenght);
void logUnsignedInt(char *name, uint32_t var, char stringLenght);
void writeNewLine();
#endif // LOGGER_H_INCLUDED
