#ifndef KEYPRESS_H
#define KEYPRESS_H

#include <stdint.h>

#define KEYPRESS_SIZE 128

struct Keypress
{
    uint8_t ascii;
    uint8_t keycode;
    uint8_t pressed;
    uint8_t shifted;
};

struct KeypressBuffer
{
    struct Keypress data[KEYPRESS_SIZE];

    uint8_t read_pointer;
    uint8_t write_pointer;
};

void keypress_clear(struct KeypressBuffer* buffer);
void keypress_push(struct KeypressBuffer* buffer, struct Keypress keypress);
struct Keypress keypress_pop(struct KeypressBuffer* buffer);

uint8_t keypress_empty(struct KeypressBuffer* buffer);

#endif