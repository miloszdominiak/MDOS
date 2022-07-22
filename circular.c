#include <circular.h>

void circular_clear(struct Circular* buffer)
{
    buffer->read_pointer = 0;
    buffer->write_pointer = 0;
    buffer->size = 0;
}

void circular_push(struct Circular* buffer, uint8_t byte)
{
    buffer->data[buffer->write_pointer] = byte;
    buffer->write_pointer = (buffer->write_pointer + 1) % CIRCULAR_SIZE;
    buffer->size++;
}

uint8_t circular_pop(struct Circular* buffer)
{
    uint8_t byte = buffer->data[buffer->read_pointer];
    buffer->read_pointer = (buffer->read_pointer + 1) % CIRCULAR_SIZE;
    buffer->size--;
    return byte;
}

uint8_t circular_empty(struct Circular* buffer)
{
    return buffer->read_pointer == buffer->write_pointer;
}