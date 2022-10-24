#include "cbuffer.h"
#include <string.h>

void CircularBuffer_Reset(struct CircularBuffer *cbuff)
{
    memset(cbuff->memblock, 0, cbuff->len);
    cbuff->_writeIndex = cbuff->_readIndex = 0;
    cbuff->_count = 0;
}

int CircularBuffer_Add(struct CircularBuffer *cbuff, const void *elem)
{
    if (CircularBuffer_Full(cbuff)) {
        return -1;
    }

    uint8_t offset = cbuff->_writeIndex * cbuff->elementSize;
    memcpy(cbuff->memblock + offset, elem, cbuff->elementSize);
    cbuff->_writeIndex = (cbuff->_writeIndex + 1) & (cbuff->len - 1);
    cbuff->_count++;

    return 0;
}

int CircularBuffer_Get(struct CircularBuffer *cbuff, void *elem)
{
    if (CircularBuffer_Empty(cbuff)) {
        return -1;
    }

    uint8_t offset = cbuff->_readIndex * cbuff->elementSize;
    memcpy(elem, cbuff->memblock + offset, cbuff->elementSize);
    cbuff->_readIndex = (cbuff->_readIndex + 1) & (cbuff->len - 1);
    cbuff->_count--;

    return 0;
}

void CircularBuffer_PeekOldest(const struct CircularBuffer *cbuff, void *elem)
{
    uint8_t offset = cbuff->_readIndex * cbuff->elementSize;
    memcpy(elem, cbuff->memblock + offset, cbuff->elementSize);
}

void CircularBuffer_PeekNewest(const struct CircularBuffer *cbuff, void *elem)
{
    uint8_t index = (cbuff->_writeIndex - 1) & (cbuff->len - 1);
    uint8_t offset = index * cbuff->elementSize;
    memcpy(elem, cbuff->memblock + offset, cbuff->elementSize);
}

bool CircularBuffer_Empty(const struct CircularBuffer *cbuff)
{
    return cbuff->_count == 0;
}

bool CircularBuffer_Full(const struct CircularBuffer *cbuff)
{
    return cbuff->_count == cbuff->len;
}
