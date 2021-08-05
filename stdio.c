#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <terminal.h>

extern uint8_t terminal_color, terminal_row, terminal_column;

void putc(char c)
{
    if(c == 0)
        return;

    if(c == '\b')
    {
        terminal_column -= 1;
        putc(' ');
        terminal_column -= 2;
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
            int digits = *format - '0';
            char buffer[20];
            puts(itoa(va_arg(list, uint32_t), buffer, digits));
        }
        format++;
    }
    va_end(list);
    set_cursor(terminal_row, terminal_column);
}