/*
 * ringbuffer.h
 *
 * Created: 20.04.2017
 * Author : Simon Bühlmann
 * Source : https://www.mikrocontroller.net/articles/FIFO
 */

#ifndef RINGBUFFER_H_INCLUDED
#define RINGBUFFER_H_INCLUDED

#include <stdint.h>

#define BUFFER_FAIL     0
#define BUFFER_SUCCESS  1

#define BUFFER_SIZE 200

typedef struct
{
    int16_t data[BUFFER_SIZE][4];
    unsigned char read; // zeigt auf das Feld mit dem ältesten Inhalt
    unsigned char write; // zeigt immer auf leeres Feld
} BufferDriveData;

unsigned char bufferIn(BufferDriveData *buffer, int16_t value1, int16_t value2, int16_t value3, int16_t value4);

unsigned char bufferOut(BufferDriveData *buffer, int16_t *pValue1, int16_t *pValue2, int16_t *pValue3, int16_t *pValue4);


#endif // RINGBUFFER_H_INCLUDED
