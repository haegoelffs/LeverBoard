#ifndef STARTUP_H_INCLUDED
#define STARTUP_H_INCLUDED

void startSynchronize(uint8_t phasestate, uint16_t time60deg);
int16_t controlCalc(int32_t target, int32_t actual);

#endif // STARTUP_H_INCLUDED
