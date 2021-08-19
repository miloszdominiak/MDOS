#ifndef STDLIB_H
#define STDLIB_H

#include <stdint.h>
#include <stddef.h>

char* itoa(uint32_t number, char* buffer, int digits);
void* malloc(size_t size);

#endif