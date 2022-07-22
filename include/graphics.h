#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stddef.h>
#include <stdint.h>

extern uint32_t* screen_buffer;
extern uint32_t* back_buffer;
extern size_t screen_width;
extern size_t screen_height;

void graphics_initialize(uint32_t* buffer, size_t width, size_t height);
void update_rect(size_t x, size_t y, size_t width, size_t height);
void button(size_t start_x, size_t start_y, size_t width, size_t height);
void draw_rect(size_t start_x, size_t start_y, size_t width, size_t height, uint32_t color);

#endif 