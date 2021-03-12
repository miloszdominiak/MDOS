#ifndef QUEUE_H
#define QUEUE_H

#include <stdint.h>

struct Queue
{
    uint8_t Size;
    uint8_t Data[100];
};

void queue_init(struct Queue* queue);
uint8_t queue_pop(struct Queue* queue);
void queue_update(struct Queue* queue);
void queue_push(struct Queue* queue, uint8_t value);
uint8_t queue_empty(struct Queue* queue);

#endif