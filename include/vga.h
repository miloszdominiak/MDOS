#ifndef VGA_H
#define VGA_H

#include <stdint.h>

void vga_init();
void vga_put_pixel(uint16_t x, uint16_t y, uint8_t color);

#endif