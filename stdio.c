#include <stdio.h>
#include <stdarg.h>
#include <terminal.h>

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
            puts(itoa(va_arg(list, int), buffer, digits));
        }
        format++;
    }
    va_end(list);
}