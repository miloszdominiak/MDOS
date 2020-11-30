#ifndef STRING_H
#define STRING_H

#include <stdint.h>

void strcpy(char* dest, char* source);
uint8_t strncmp(const char* first, const char* second, uint16_t size);

#endif