#include <terminal.h>
#include <stdio.h>
#include <gdt.h>
#include <idt.h>
#include <pci.h>
#include <threads.h>
#include <irq.h>
#include <pic.h>
#include <stdlib.h>
#include <ps2.h>
#include <keypress.h>


uint32_t count = 0;

extern struct ThreadInfo* first_ready_to_run;
extern struct ThreadInfo* last_ready_to_run;

extern struct KeypressBuffer keypress_buffer;
extern uint8_t key_state[];

void timer()
{
    count++;
    pic_master_eoi();
    if(!(count % 5))
    {
        //printf("-");
        scheduler_lock();
        schedule();
        scheduler_unlock();
    }  
}

void kernel_main()
{
    set_color(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_GREY);
    terminal_initialize();

    printf("MDOS\n");
    printf("----\n");

    gdt_init();
    idt_init();
    pci_enum();

    threads_init();

    irq_install_handler(0, timer);

    ps2_controller_init();

    while(1)
    {
        // if(!keypress_empty(&keypress_buffer))
        // {
        //     struct Keypress key = keypress_pop(&keypress_buffer);
        //     if(key.pressed)
        //         putc(key.ascii);
        //     update_cursor();
        // }
        while(key_state[0x01])
        {
            printf("a");

        for(int i = 0; i < 1000000; i++);
        }
        for(int i = 0; i < 1000000; i++);
        printf("czekam ");
    }
}