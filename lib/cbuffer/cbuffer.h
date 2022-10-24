/**
 * Библиотека, реализующая циклический буфер.
 * 
 * Поддерживаются размеры, являющиеся степенями 2.
 */

#ifndef _CBUFFER_H
#define _CBUFFER_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

struct CircularBuffer {
    void *const memblock;
    const uint8_t elementSize;
    const uint8_t len;
    volatile uint8_t _writeIndex;
    uint8_t _readIndex;
    uint8_t _count;
};

void CircularBuffer_Reset(struct CircularBuffer *cbuff);
int CircularBuffer_Add(struct CircularBuffer *cbuff, const void *elem);
int CircularBuffer_Get(struct CircularBuffer *cbuff, void *elem);
void CircularBuffer_PeekOldest(const struct CircularBuffer *cbuff, void *elem);
void CircularBuffer_PeekNewest(const struct CircularBuffer *cbuff, void *elem);
bool CircularBuffer_Empty(const struct CircularBuffer *cbuff);
bool CircularBuffer_Full(const struct CircularBuffer *cbuff);

#define CircularBuffer_GetElements(cbuff)   ((cbuff)->_count)

#endif // _CBUFFER_H