#ifndef STARTUP_H_INCLUDED
#define STARTUP_H_INCLUDED

void startControlled(uint8_t phasestate_init, uint16_t time60deg_init, void (*tooSlowCallback)(void));
void stopControlled(void);

#endif // STARTUP_H_INCLUDED
