#include <stdint.h>
#include <stddef.h>
#include <colors.h>

uint8_t color(enum vga_color bg, enum vga_color fg);
void set_color(enum vga_color bg, enum vga_color fg);

void disable_cursor();

void put_char_at(char c, uint8_t color, size_t y, size_t x);

void terminal_initialize();

void putc(char c);
void puts(const char* str);