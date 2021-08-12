#include <keypress.h>
#include <stdio.h>

void keypress_clear(struct KeypressBuffer* buffer)
{
    buffer->read_pointer = 0;
    buffer->write_pointer = 0;
}

void keypress_push(struct KeypressBuffer* buffer, struct Keypress keypress)
{
    buffer->data[buffer->write_pointer] = keypress;
    buffer->write_pointer = (buffer->write_pointer + sizeof(struct Keypress)) % KEYPRESS_SIZE;
}

struct Keypress keypress_pop(struct KeypressBuffer* buffer)
{
    struct Keypress byte = buffer->data[buffer->read_pointer];
    buffer->read_pointer = (buffer->read_pointer + sizeof(struct Keypress)) % KEYPRESS_SIZE;
    return byte;
}

uint8_t keypress_empty(struct KeypressBuffer* buffer)
{
    return buffer->read_pointer == buffer->write_pointer;
}