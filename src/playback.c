#include "playback.h"

static struct FiniteTone finiteTones[2] = {0};
#define QUEUE_LEN   (sizeof(finiteTones) / sizeof(finiteTones[0]))
#define INDEX_MASK  (QUEUE_LEN - 1)

static uint8_t count = 0;
static uint8_t writeIndex = 0;
static uint8_t readIndex = 0;

void Playback_ResetQueue(void)
{
    count = readIndex = writeIndex = 0;
}

void Playback_Enqueue(struct FiniteTone finiteTone)
{
    if (!Playback_QueueFull()) {
        finiteTones[writeIndex++] = finiteTone;
        writeIndex &= INDEX_MASK;
        count++;
    }
}

struct FiniteTone Playback_Dequeue(void)
{
    struct FiniteTone finiteTone;

    if (!Playback_QueueEmpty()) {
        finiteTone = finiteTones[readIndex++];
        readIndex &= INDEX_MASK;
        count--;
    } else {
        finiteTone = (struct FiniteTone){0};
    }
    return finiteTone;
}

bool Playback_QueueEmpty(void)
{
    return count == 0;
}

bool Playback_QueueFull(void)
{
    return count == QUEUE_LEN;
}
