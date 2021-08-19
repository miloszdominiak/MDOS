#ifndef THREADS_H
#define THREADS_H

#include <stdint.h>

struct ThreadInfo
{
    uint32_t *esp;
    struct ThreadInfo *next;
    uint8_t state;
    char *name;
};

typedef void (*ThreadFunction)();

struct ThreadInfo* threads_init();
struct ThreadInfo* thread_create(ThreadFunction function);
void thread_switch(struct ThreadInfo* next_thread);
void thread_next();

#endif