#include <string.h>
#include <stddef.h>

static void swap(size_t *first, size_t *second)
{
    size_t buffer = *second;
    *second = *first;
    *first = buffer;
}

void strcpy(char* dest, char* source)
{
    while(*(source))
        *(dest++) = *(source++);
    *dest = 0;
}

size_t strlen(const char* first)
{
    size_t length = 0;
    while(*first++)
        length++;
        
    return length;
}

int8_t strcmp(const char* first, const char* second)
{
    size_t length1 = strlen(first);
    size_t length2 = strlen(second);

    if(length1 > length2)
        swap(&length1, &length2);
    
    for(size_t i = 0; i < length1; i++)
        if(first[i] < second[i])
            return -1;
        else if(first[i] > second[i])
            return 1;
    
    return 0;
}

int8_t strncmp(const char* first, const char* second, uint16_t size)
{
    for(int i = 0; i < size; i++)
        if(first[i] < second[i])
            return -1;
        else if(first[i] > second[i])
            return 1;

    return 0;
}

int8_t memcmp(void* f, void* s, uint16_t size)
{
    char* first = f;
    char* second = s;
    for(int i = 0; i < size; i++)
        if(first[i] < second[i])
            return -1;
        else if(first[i] > second[i])
            return 1;

    return 0;
}