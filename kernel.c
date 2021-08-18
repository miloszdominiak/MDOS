#include <terminal.h>
#include <stdio.h>
#include <gdt.h>
#include <idt.h>
#include <pci.h>

void kernel_main()
{
    set_color(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_CYAN);
    terminal_initialize();

    printf("MDOS\n");
    printf("----\n");

    gdt_init();
    idt_init();
    pci_enum();
}