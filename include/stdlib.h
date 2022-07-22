#ifndef STDLIB_H
#define STDLIB_H

#include <stdint.h>
#include <stddef.h>

char* itoa(uint32_t number, char* buffer, int digits);
void* malloc(size_t size);
int strtol(const char* str, const char* endptr, int base);
int rand();

#endif