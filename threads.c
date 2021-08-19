#include <threads.h>
#include <stdlib.h>

struct ThreadInfo* current_thread;

struct ThreadInfo* threads_init()
{
    struct ThreadInfo* thread_info = malloc(sizeof(struct ThreadInfo));
    current_thread = thread_info;
    return thread_info;
}

struct ThreadInfo* thread_create(ThreadFunction function)
{
    struct ThreadInfo* thread_info = malloc(sizeof(struct ThreadInfo));
    
    uint32_t* thread_stack = malloc(512) + 512;
    (*--thread_stack) = (uint32_t)function;
    thread_stack -= 4;

    thread_info->esp = thread_stack;

    return thread_info;
}

#include <stdio.h>
void thread_next()
{
    thread_switch(current_thread->next);
}