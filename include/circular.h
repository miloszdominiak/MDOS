#ifndef CIRCULAR_H
#define CIRCULAR_H

#include <stdint.h>

#define CIRCULAR_SIZE 5

struct Circular
{
    uint8_t data[CIRCULAR_SIZE];

    uint8_t read_pointer;
    uint8_t write_pointer;
};

void circular_clear(struct Circular* buffer);
void circular_push(struct Circular* buffer, uint8_t byte);
uint8_t circular_pop(struct Circular* buffer);

uint8_t circular_empty(struct Circular* buffer);

#endif