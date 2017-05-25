#include <stdint.h>
#include "ringbufferDriveData.h"
#include "logger.h"


unsigned char bufferIn(BufferDriveData *buffer, int16_t value1, int16_t value2, int16_t value3, int16_t value4)
{
    if((buffer->write + 1)%BUFFER_SIZE == buffer->read)
    {
        // buffer full
        return BUFFER_FAIL;
    }

    // write data in array
    buffer->data[buffer->write][0] = value1;
    buffer->data[buffer->write][1] = value2;
    buffer->data[buffer->write][2] = value3;
    buffer->data[buffer->write][3] = value4;

    // increase write pointer
    buffer->write = (buffer->write + 1)%BUFFER_SIZE;

    return BUFFER_SUCCESS;
}

unsigned char bufferOut(BufferDriveData *buffer, int16_t *pValue1, int16_t *pValue2, int16_t *pValue3, int16_t *pValue4)
{
    if (buffer->read == buffer->write)
    {
        // no element
        return BUFFER_FAIL;
    }


    *pValue1 = buffer->data[buffer->read][0];
    *pValue2 = buffer->data[buffer->read][1];
    *pValue3 = buffer->data[buffer->read][2];
    *pValue4 = buffer->data[buffer->read][3];

    buffer->read = (buffer->read + 1)%BUFFER_SIZE;

    return BUFFER_SUCCESS;
}

