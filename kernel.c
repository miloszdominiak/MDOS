#include <terminal.h>
#include <stdio.h>
#include <gdt.h>
#include <interrupts.h>
#include <ps2.h>
#include <circular.h>

char scancode_to_character[] = 
{ 0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', '\t',
'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0, 'a', 's',
'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 'v',
'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, '7', '8', '9', '-', '4', '5', '6', '+', '1',
'2', '3', '0', '.'};

extern uint8_t terminal_color, terminal_row, terminal_column;

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
    {
        while(!circular_empty(&keyboard_buffer))
        {
            putc(scancode_to_character[circular_pop(&keyboard_buffer)]);
            set_cursor(terminal_row, terminal_column);
            asm("hlt");
        }
    }

    while(1)
        asm("hlt");
}