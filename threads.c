#include <threads.h>
#include <stdlib.h>
#include <irq.h>
#include <pic.h>
#include <stdio.h>

uint32_t count = 0;

void timer()
{
    count++;
    pic_master_eoi();

    //printf("%1", count);
    
    scheduler_lock();
    schedule();
    scheduler_unlock();
}

struct ThreadInfo* current_thread;
struct ThreadInfo* first_ready_to_run;
struct ThreadInfo* last_ready_to_run;

uint32_t cli_lock;

struct ThreadInfo* threads_init()
{
    irq_install_handler(0, timer);
    struct ThreadInfo* thread_info = malloc(sizeof(struct ThreadInfo));
    current_thread = thread_info;
    thread_info->state = THREAD_STATE_RUNNING;
    thread_info->next = 0;
    thread_info->name = "KERNEL";
    return thread_info;
}

struct ThreadInfo* thread_create(ThreadFunction function, const char* name)
{
    struct ThreadInfo* thread_info = malloc(sizeof(struct ThreadInfo));
    
    uint32_t* thread_stack = malloc(512) + 512;
    (*--thread_stack) = (uint32_t)function;
    thread_stack -= 4;

    thread_info->esp = thread_stack;
    thread_info->next = 0;
    thread_info->name = name;

    scheduler_push(thread_info);

    thread_info->state = THREAD_STATE_READY_TO_RUN;
    return thread_info;
}

void thread_block()
{
    current_thread->state = THREAD_STATE_BLOCKED;
    scheduler_lock();
    schedule();
    scheduler_unlock();
}

void thread_unblock(struct ThreadInfo* thread)
{
    if(thread->state == THREAD_STATE_BLOCKED)
    {
        thread->state = THREAD_STATE_READY_TO_RUN;
        //scheduler_push(thread);
        scheduler_lock();
        thread_switch(thread);
        scheduler_unlock();
    }
}

void thread_ready()
{
    if(current_thread->state == THREAD_STATE_RUNNING)
    {
        current_thread->state = THREAD_STATE_READY_TO_RUN;
        scheduler_push(current_thread);
    }
}

void thread_running()
{
    current_thread->state = THREAD_STATE_RUNNING;
}

void schedule()
{
    if(first_ready_to_run)
    {
        struct ThreadInfo* next_thread = first_ready_to_run;
        first_ready_to_run = first_ready_to_run->next;
        //puts(next_thread->name);

        thread_switch(next_thread);
        //printf("---%1---", cli_lock);
    }
}

void scheduler_push(struct ThreadInfo* thread)
{
    if(first_ready_to_run)
        last_ready_to_run->next = thread;
    else
        first_ready_to_run = thread;

    last_ready_to_run = thread;
}

void scheduler_lock()
{
    asm("cli");
    cli_lock++;
}

void scheduler_unlock()
{
    cli_lock--;
    if(!cli_lock)
        asm("sti");
}