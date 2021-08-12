#include <terminal.h>
#include <stdio.h>
#include <gdt.h>
#include <idt.h>
#include <ps2.h>
#include <circular.h>
#include <pci.h>
#include <acpi.h>
#include <keypress.h>
#include <ps2kbd.h>

char scancode_to_character[] = 
{ 0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', '\t',
'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0, 'a', 's',
'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 'v',
'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, '7', '8', '9', '-', '4', '5', '6', '+', '1',
'2', '3', '0', '.'};

extern uint8_t terminal_color, terminal_row, terminal_column;

extern struct Circular scancode_buffer;
extern struct KeypressBuffer keypress_buffer;

void kernel_main()
{
    set_color(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_CYAN);
    terminal_initialize();

    printf("MDOS\n");
    printf("----\n");

    gdt_init();
    idt_init();

    pci_enum();

    if(is_ps2_present())
    {
        printf("PS2 controller present\n");
        ps2_controller_init();
    }

    while(1)
    {
        for(int i = 0; i < 100000000; i++)
            if(!(i % 10000000))
                printf("%1 ", i / 10000000);
        printf("\n");
        putc(scanf());
    }

    while(1)
        asm("hlt");
}