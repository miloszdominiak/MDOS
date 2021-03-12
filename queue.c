#include <queue.h>

void queue_init(struct Queue* queue)
{
    queue->Size = 0;
}

uint8_t queue_pop(struct Queue* queue)
{
    uint8_t front = queue->Data[0];
    queue_update(queue);
    return front;
}

void queue_update(struct Queue* queue)
{
    for(int i = 1; i < queue->Size; i++)
        queue->Data[i - 1] = queue->Data[i];

    queue->Size--;
}

void queue_push(struct Queue* queue, uint8_t value)
{
    queue->Data[queue->Size] = value;
    queue->Size++;
}

uint8_t queue_empty(struct Queue* queue)
{
    return !queue->Size;
}