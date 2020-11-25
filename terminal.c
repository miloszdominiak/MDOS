#include <terminal.h>
#include <ports.h>

#define VGA_BUFFER 0xB8000;

#define COLOR(bg,fg) bg << 4 | fg;
#define VGA_ENTRY(c,color) color << 8 | c

static const size_t VGA_WIDTH = 80, VGA_HEIGHT = 25;

static uint8_t terminal_color = COLOR(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_GREY);
static size_t terminal_row, terminal_column;

inline uint8_t color(enum vga_color bg, enum vga_color fg)
{
    return COLOR(bg, fg);
}

inline void set_color(enum vga_color bg, enum vga_color fg)
{
    terminal_color = COLOR(bg, fg);
}

void put_char_at(char c, uint8_t color, size_t y, size_t x)
{
    uint16_t* buffer = (uint16_t*) VGA_BUFFER;
    buffer[y * VGA_WIDTH + x] = VGA_ENTRY(c, color);
}

void set_cursor(size_t y, size_t x)
{
    uint16_t position = VGA_WIDTH * y + x;
    outb(0x3D4, 0xF);
    outb(0x3D5, (uint8_t)position);
    outb(0x3D4, 0xE);
    outb(0x3D5, position >> 8);
}

void terminal_initialize()
{   
    for(size_t y = 0; y < VGA_HEIGHT; y++)
        for(size_t x = 0; x < VGA_WIDTH; x++)
            put_char_at(' ', terminal_color, y, x);
    
    terminal_row = terminal_column = 0;
    set_cursor(terminal_row, terminal_column);
}

void putc(char c)
{
    if(c != '\n')
        put_char_at(c, terminal_color, terminal_row, terminal_column);
    else
    {
        terminal_column = 0;
        terminal_row++;
    }
    if(c != '\n' && ++terminal_column == 80)
    {
        terminal_column = 0;
        terminal_row++;
    }
}

void puts(const char* str)
{
    do
    {
        putc(*str);
    }
    while(*(++str));
    set_cursor(terminal_row, terminal_column);
}