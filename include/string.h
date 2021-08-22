#ifndef STRING_H
#define STRING_H

#include <stdint.h>
#include <stddef.h>

size_t strlen(const char* first);
int8_t strcmp(const char* first, const char* second);
void strcpy(char* dest, char* source);
int8_t strncmp(const char* first, const char* second, uint16_t size);
int8_t memcmp(void* first, void* second, uint16_t size);

char* strtok(char* str, char delimiter);

#endif