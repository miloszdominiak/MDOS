#include <string.h>

void strcpy(char* dest, char* source)
{
    while(*(source))
        *(dest++) = *(source++);
    *dest = 0;
}