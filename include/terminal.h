#ifndef TERMINAL_H
#define TERMINAL_H

#include <colors.h>
#include <stdint.h>

extern uint8_t terminal_color, terminal_row, terminal_column;

void set_color(enum vga_color bg, enum vga_color fg);

void set_cursor(uint8_t y, uint8_t x);
void disable_cursor();
void update_cursor();

void put_char_at(char c, uint8_t color, uint8_t y, uint8_t x);

void terminal_initialize();

#endif