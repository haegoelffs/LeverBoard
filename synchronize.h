#ifndef STARTUP_H_INCLUDED
#define STARTUP_H_INCLUDED

#define MEASURE

void startSynchronize(uint8_t phasestate_init, uint16_t time60deg_init, void (*tooSlowCallback)(void));
int16_t controlCalc(int32_t target, int32_t actual);

#endif // STARTUP_H_INCLUDED
