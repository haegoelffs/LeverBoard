/*
last change: 08.5.2017
version: 0.1
*/

#ifndef DRIVE_H_INCLUDED
#define DRIVE_H_INCLUDED

#include <stdint.h>

typedef enum {
            free_running,
            fix_commutated,
            controlled
            } DriveState;

void initDrive(void);

void setPowerLevel(int8_t power);

#endif // DRIVE_H_INCLUDED
