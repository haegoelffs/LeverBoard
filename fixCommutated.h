#ifndef STARTUP_H_INCLUDED
#define STARTUP_H_INCLUDED

void startSpeedUp(void (*startupFinishedCallback)(uint8_t, uint16_t));
void stopSpeedUp();

#endif // STARTUP_H_INCLUDED
