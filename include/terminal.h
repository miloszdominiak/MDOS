#ifndef TERMINAL_H
#define TERMINAL_H

#include <colors.h>
#include <stdint.h>
#include <stdbool.h>

extern uint32_t terminal_color, terminal_row, terminal_column;
extern uint32_t screen_width, screen_height;
extern uint32_t terminal_rows, terminal_columns;
extern bool terminal_echo;

void set_color(enum vga_color bg, enum vga_color fg);

void set_cursor(uint8_t y, uint8_t x);
void disable_cursor();
void update_cursor();

void put_character_at(uint8_t character, uint32_t color, uint8_t char_y, uint8_t char_x);

void terminal_initialize(uint32_t buffer, uint32_t screen_width, uint32_t screen_height);

void terminal_scroll();
void terminal_clear();

void terminal_change_color();

#endif