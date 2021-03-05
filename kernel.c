#include <terminal.h>
#include <stdio.h>
#include <ps2.h>
extern void interrupts_init();
#include <gdt.h>

void kernel_main()
{
    set_color(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_CYAN);
    terminal_initialize();

    printf("MDOS\n");
    printf("----\n");

    

    ps2_controller_init();
    gdt_init();
    interrupts_init();

    
}