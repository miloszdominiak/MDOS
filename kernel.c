#include <terminal.h>
#include <stdio.h>
#include <gdt.h>
#include <interrupts.h>
#include <ps2.h>

void kernel_main()
{
    set_color(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_CYAN);
    terminal_initialize();

    printf("MDOS\n");
    printf("----\n");
    
    gdt_init();
    interrupts_init();

    ps2_controller_init();

    while(1)
        asm("hlt");
}