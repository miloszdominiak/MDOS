#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <terminal.h>
#include <circular.h>
#include <keypress.h>
#include <ps2kbd.h>

extern struct KeypressBuffer keypress_buffer;

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

uint8_t scanf(const char* format, ...)
{
    va_list list;
    va_start(list, format);

    while(*format)
    {
        if(*format == '%')
        {
            format++;
            if(*format == 'c')
            {
                char* dest = va_arg(list, char*);
                while(1)
                {
                    while(keypress_empty(&keypress_buffer))
                        asm("hlt");

                    struct Keypress keypress = keypress_pop(&keypress_buffer);
                
                    if(keypress.ascii && keypress.pressed)
                    {
                        *dest = keypress.ascii;
                        return 1;
                    }
                }
            }
            else if(*format == 's')
            {
                char* dest = va_arg(list, char*);
                char znak;
                do
                {
                    scanf("%c", &znak);
                    if(znak == '\b')
                        *dest-- = 0;
                    else
                        *dest++ = znak;
                } while(znak != '\n');
                *dest = 0;  
            }
        }
        format++;
    }
    va_end(list);
    return 0;
}