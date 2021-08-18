#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <terminal.h>
#include <circular.h>
#include <keypress.h>
#include <ps2kbd.h>

extern struct Circular scancode_buffer;
extern struct KeypressBuffer keypress_buffer;

extern uint8_t terminal_color, terminal_row, terminal_column;

void putc(char c)
{
    if(c == 0)
        return;

    if(c == '\b')
    {
        if(terminal_column != 0)
        {
        terminal_column -= 1;
        putc(' ');
        terminal_column -= 2;
        }
        else
        {
            terminal_row--;
            terminal_column = 79;
            putc(' ');
            terminal_row--;
            terminal_column = 78;
        }
    }
    else if(c == '\n')
    {
        terminal_column = 0;
        terminal_row++;
    }
    else
        put_char_at(c, terminal_color, terminal_row, terminal_column);
    
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

void printf(const char* format, ...)
{
    va_list list;
    va_start(list, format);

    while(*format)
    {
        if(*format != '%')
            putc(*format);
        else
        {
            format++;
            if(*format != 'c')
            {
                int digits = *format - '0';
                char buffer[20];
                puts(itoa(va_arg(list, uint32_t), buffer, digits));
            }
            else
            {
                putc((uint8_t)va_arg(list, uint32_t));
                update_cursor();
            }
        }
        format++;
    }
    va_end(list);
    set_cursor(terminal_row, terminal_column);
}