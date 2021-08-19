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

// struct ThreadInfo kernelthread;
// struct ThreadInfo next;

struct ThreadInfo* current_thread_info;

// int switch_thread(struct ThreadInfo* next_thread_info);
// extern uint32_t drugi_stack;

void kernel_main()
{
    set_color(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_GREY);
    terminal_initialize();

    printf("MDOS\n");
    printf("----\n");

    gdt_init();
    idt_init();
    pci_enum();

    printf("%4\n", malloc(5));
    printf("%4\n", malloc(1));
    printf("%4\n", malloc(1));
    printf("%4\n", malloc(1));

    //irq_install_handler(0, timer);
    // irq_install_handler(1, klawiatura);
    // uint32_t* wskaznik_na_stack = &drugi_stack;
    // wskaznik_na_stack--;

    // *wskaznik_na_stack = (uint32_t)druga_funkcja;
    // wskaznik_na_stack -= 4;

    // next.stack_address = (uint32_t)wskaznik_na_stack;
    // set_color(VGA_COLOR_BROWN, VGA_COLOR_BLACK);
    // int dupa = 0;
    // while(1)
    // {
    //     for(int i = 0; i < 100000000; i++)
    //         ;
        

    //      printf("%1 ", dupa++);
    // }
    // while(1)
    //     asm("hlt");
    
}