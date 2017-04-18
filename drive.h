#ifndef DRIVE_H_INCLUDED
#define DRIVE_H_INCLUDED

#include <stdint.h>

void initDrive();

void setRotationsPerSec(uint16_t rotations);
uint16_t getActualRotationsPerSec();

void setBreakCoefficient(char coefficient);

#endif // DRIVE_H_INCLUDED
