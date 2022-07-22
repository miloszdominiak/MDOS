#ifndef THREADS_H
#define THREADS_H

#include <stdint.h>

struct ThreadInfo
{
    uint32_t *esp;
    struct ThreadInfo *next;
    uint8_t state;
    const char *name;
};

enum ThreadState
{
    THREAD_STATE_BLOCKED,
    THREAD_STATE_READY_TO_RUN,
    THREAD_STATE_RUNNING
};

typedef void (*ThreadFunction)();

struct ThreadInfo* threads_init();
struct ThreadInfo* thread_create(ThreadFunction function, const char* name);
void thread_switch(struct ThreadInfo* next_thread);
void thread_block();
void thread_unblock(struct ThreadInfo* thread);
void schedule();
void scheduler_push();
void scheduler_lock();
void scheduler_unlock();

#endif