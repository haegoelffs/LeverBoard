#ifndef LOGGERISR_H_INCLUDED
#define LOGGERISR_H_INCLUDED

void logMsgBuffered(char *msg);
void logUnsignedIntBuffered(uint16_t var, char stringLenght);
void logNamedUnsignedIntBuffered(char *name, uint16_t var, char stringLenght);
void logSignedIntBuffered(int16_t var, char stringLenght);
void logNamedSignedIntBuffered(char *name, uint16_t var, char stringLenght);
void logSignedIntLineBuffered(int16_t var1, int16_t var2, int16_t var3, int16_t var4, char stringLenght);

void writeBuffered();

#endif // LOGGERISR_H_INCLUDED
