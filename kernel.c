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
#include <string.h>

uint32_t count = 0;

extern struct ThreadInfo* first_ready_to_run;
extern struct ThreadInfo* last_ready_to_run;

extern struct KeypressBuffer keypress_buffer;

char *dupa = 0;

void timer()
{
    count++;
    pic_master_eoi();
    if(!(count % 10))
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
    printf("%1", dupa);

    gdt_init();
    idt_init();
    pci_enum();

    threads_init();

    irq_install_handler(0, timer);

    ps2_controller_init();

    char line[50];
    while(1)
    {
        printf(">");
        scanf("%s", line);
        char* command = strtok(line, ' ');
        printf(command);
        
        // if(!strcmp(command, "inb"))
        // {
        //     //char* argument = strtok(NULL, '\n');

        //     //printf("%1", strtol(argument, NULL, 16) * 2);
        // }
        // else if(!strcmp(command, "outb"))
        // {
        // }
    }
}