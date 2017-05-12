/*
 * ringbuffer.c
 *
 * Created: 20.04.2017
 * Author : Simon Bühlmann
 * Source : https://www.mikrocontroller.net/articles/FIFO
 */
#include <stdint.h>

#include "ringbuffer.h"

struct Buffer
{
    uint16_t data[BUFFER_SIZE];
    unsigned char read; // zeigt auf das Feld mit dem ältesten Inhalt
    unsigned char write; // zeigt immer auf leeres Feld
} buffer = {{}, 0, 0};

// Stellt 1 Byte in den Ringbuffer
//
// Returns:
//     BUFFER_FAIL       der Ringbuffer ist voll. Es kann kein weiteres Byte gespeichert werden
//     BUFFER_SUCCESS    das Byte wurde gespeichert
//
unsigned char bufferIn(uint16_t byte)
{
    if(((buffer.write + 1) == buffer.read) ||
        ((buffer.read == 0) && ((buffer.write + 1) == BUFFER_SIZE)))
    {
        return BUFFER_FAIL; // buffer full
    }

    // write data in array
    buffer.data[buffer.write] = byte;

    // increase write pointer
    buffer.write++;


    if (buffer.write >= BUFFER_SIZE)
    {
        // "wrap around" pointer to first array element (ringbuffer)
        buffer.write = 0;
    }

    return BUFFER_SUCCESS;
}

// Holt 1 Byte aus dem Ringbuffer, sofern mindestens eines abholbereit ist
//
// Returns:
//     BUFFER_FAIL       der Ringbuffer ist leer. Es kann kein Byte geliefert werden.
//     BUFFER_SUCCESS    1 Byte wurde geliefert
//
unsigned char bufferOut(uint16_t *pByte)
{
    if (buffer.read == buffer.write)
    {
        // no element
        return BUFFER_FAIL;
    }

    *pByte = buffer.data[buffer.read];

    // increase read pointer
    buffer.read++;

    if (buffer.read >= BUFFER_SIZE)
    {
        // "wrap around" pointer to first array element (ringbuffer)
        buffer.read = 0;
    }

    return BUFFER_SUCCESS;
}
