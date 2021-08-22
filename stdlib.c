#include <stdlib.h>

#include <stdio.h>

extern uint8_t freememory;

uint8_t* free_memory_pointer = &freememory;

void* malloc(uint32_t size)
{
    void* memory_pointer = free_memory_pointer;
    free_memory_pointer += size;
    return memory_pointer;
}

static char dtoh(uint8_t digit)
{
    switch(digit)
    {
    case 10:
        return 'A';
        
    case 11:
        return 'B';
        
    case 12:
        return 'C';
        
    case 13:
        return 'D';
        
    case 14:
        return 'E';

    case 15:
        return 'F';
        
    default:
        return digit + '0';
    }
}

char* itoa(uint32_t number, char* buffer, int bytes)
{
    int digits = bytes * 2;
    buffer[digits] = '\0';
    for(int i = digits - 1; i >= 0; i--)
    {
        buffer[i] = dtoh(number % 16);
        number /= 16;
    }
    return buffer;
}

static int digit_to_int(char digit)
{
    if(digit >= '0' && digit <= '9')
        return digit - '0';
    else if(digit >= 'a' && digit <= 'z')
        return digit - 'a' + 10;
    else
        return digit - 'A' + 10;
}

int strtol(const char* str, const char* endptr, int base)
{
    (void)endptr;
    int result = 0;
    while(*str)
    {
        result *= base;
        result += digit_to_int(*str++);
    }
    return result;
}