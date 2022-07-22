#ifndef STDIO_H
#define STDIO_H

#include <stdint.h>
#include <stddef.h>

void putc(char c);
void puts(const char* str);
void printf(const char* format, ...);
uint8_t scanf(const char* format, ...);
size_t getline(char** buffer);

#endif