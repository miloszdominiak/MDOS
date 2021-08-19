#include <terminal.h>
#include <stdio.h>
#include <gdt.h>
#include <idt.h>
#include <pci.h>
#include <threads.h>
#include <irq.h>
#include <pic.h>
#include <stdlib.h>

uint32_t count = 0;

void druga()
{
    while(1)
    {
        printf("b");
        thread_next();
    }
}

void trzecia()
{
    while(1)
    {
        printf("c");
        thread_next();
    }
}

extern struct ThreadInfo* current_thread;

void kernel_main()
{
    set_color(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_GREY);
    terminal_initialize();

    printf("MDOS\n");
    printf("----\n");

    gdt_init();
    idt_init();
    pci_enum();

    struct ThreadInfo* kernel_thread = threads_init();
    struct ThreadInfo* drugi_thread = thread_create(druga);
    struct ThreadInfo* trzeci_thread = thread_create(trzecia);
    
    kernel_thread->name = "kernel\n";
    drugi_thread->name = "drugi\n";
    trzeci_thread->name = "trzeci\n";
    
    kernel_thread->next = drugi_thread;
    drugi_thread->next = trzeci_thread;
    trzeci_thread->next = kernel_thread;

    
    for(int i = 0; i < 3; i++)
    {
        printf("a");
        thread_next();
        printf("\n");
    }
}