#include <stdint.h>
#include "logger.h"
#include "ringbuffer.h"

struct Buffer
{
    uint16_t data[BUFFER_SIZE];
    unsigned char read; // zeigt auf das Feld mit dem ältesten Inhalt
    unsigned char write; // zeigt immer auf leeres Feld
} buffer = {{}, 0, 0};

unsigned char bufferIn(uint16_t byte)
{
    if((buffer.write + 1)%BUFFER_SIZE == buffer.read)
    {
        // buffer full
        return BUFFER_FAIL;
    }

    // write data in array
    buffer.data[buffer.write] = byte;

    // increase write pointer
    buffer.write = (buffer.write + 1)%BUFFER_SIZE;

    return BUFFER_SUCCESS;
}

unsigned char bufferOut(uint16_t *pByte)
{
    if (buffer.read == buffer.write)
    {
        // no element
        return BUFFER_FAIL;
    }

    *pByte = buffer.data[buffer.read];

    buffer.read = (buffer.read + 1)%BUFFER_SIZE;

    return BUFFER_SUCCESS;
}
