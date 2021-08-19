#include <terminal.h>
#include <stdio.h>
#include <gdt.h>
#include <idt.h>
#include <pci.h>
#include <threads.h>
#include <irq.h>
#include <pic.h>

uint32_t count = 0;

struct ThreadInfo kernelthread;
struct ThreadInfo next;

struct ThreadInfo* current_thread_info = &kernelthread;

int switch_thread(struct ThreadInfo* next_thread_info);

extern uint32_t drugi_stack;

void druga_funkcja()
{
    int dupa = 0;
    while(1)
    {
        for(int i = 0; i < 100000000; i++)
            ;
        printf("%1 ", dupa+=2);
    }
}

uint32_t bimanie = 1;
void timer()
{
    asm("cli");
    pic_master_eoi();
    if(!(count++ % 100))
    {
        if(kernelthread.state)
        {
            kernelthread.state = 0;
            next.state = 1;
            set_color(VGA_COLOR_RED, VGA_COLOR_WHITE);
            //terminal_initialize();
            switch_thread(&next);
        }
        else
        {
            kernelthread.state = 1;
            next.state = 0;
            set_color(VGA_COLOR_BROWN, VGA_COLOR_BLACK);
            //terminal_initialize();
            switch_thread(&kernelthread);
        }
    }
}

int ile_wcisniec = 0;

#include <ports.h>

void klawiatura()
{
    asm("cli");
    pic_master_eoi();
    inb(0x60);
    ile_wcisniec++;

    if(!(ile_wcisniec % 2))
    {
        if(kernelthread.state)
        {
            kernelthread.state = 0;
            next.state = 1;
            set_color(VGA_COLOR_RED, VGA_COLOR_WHITE);
            //terminal_initialize();
            switch_thread(&next);
        }
        else
        {
            kernelthread.state = 1;
            next.state = 0;
            set_color(VGA_COLOR_BROWN, VGA_COLOR_BLACK);
            //terminal_initialize();
            switch_thread(&kernelthread);
        }
    }
}

void kernel_main()
{
    set_color(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_CYAN);
    terminal_initialize();

    printf("MDOS\n");
    printf("----\n");

    gdt_init();
    idt_init();
    pci_enum();

    //irq_install_handler(0, timer);
    irq_install_handler(1, klawiatura);
    uint32_t* wskaznik_na_stack = &drugi_stack;
    wskaznik_na_stack--;

    *wskaznik_na_stack = (uint32_t)druga_funkcja;
    wskaznik_na_stack -= 4;

    next.stack_address = (uint32_t)wskaznik_na_stack;
    set_color(VGA_COLOR_BROWN, VGA_COLOR_BLACK);
    int dupa = 0;
    while(1)
    {
        for(int i = 0; i < 100000000; i++)
            ;
        

         printf("%1 ", dupa++);
    }
    while(1)
        asm("hlt");
    
}