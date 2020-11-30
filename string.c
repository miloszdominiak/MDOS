#include <string.h>

void strcpy(char* dest, char* source)
{
    while(*(source))
        *(dest++) = *(source++);
    *dest = 0;
}

uint8_t strncmp(const char* first, const char* second, uint16_t size)
{
    for(int i = 0; i < size; i++)
        if(first[i] != second[i])
            return 1;
    return 0;
}