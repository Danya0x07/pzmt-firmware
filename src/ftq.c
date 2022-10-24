#include "ftq.h"

static struct FiniteTone finiteTones[2] = {0};
#define QUEUE_LEN   (sizeof(finiteTones) / sizeof(finiteTones[0]))
#define INDEX_MASK  (QUEUE_LEN - 1)

static uint8_t count = 0;
static uint8_t writeIndex = 0;
static uint8_t readIndex = 0;

void Ft_ResetQueue(void)
{
    count = readIndex = writeIndex = 0;
}

void Ft_Enqueue(struct FiniteTone finiteTone)
{
    if (!Ft_QueueFull()) {
        finiteTones[writeIndex++] = finiteTone;
        writeIndex &= INDEX_MASK;
        count++;
    }
}

struct FiniteTone Ft_Dequeue(void)
{
    struct FiniteTone finiteTone;

    if (!Ft_QueueEmpty()) {
        finiteTone = finiteTones[readIndex++];
        readIndex &= INDEX_MASK;
        count--;
    } else {
        finiteTone = (struct FiniteTone){0};
    }
    return finiteTone;
}

bool Ft_QueueEmpty(void)
{
    return count == 0;
}

bool Ft_QueueFull(void)
{
    return count == QUEUE_LEN;
}
