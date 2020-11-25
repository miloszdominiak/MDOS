#include <terminal.h>
#include <ports.h>
#include <pci.h>
#include <string.h>
#include <stdio.h>

void kernel_main()
{
    set_color(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_CYAN);
    terminal_initialize();

    printf("MDOS\n");
    printf("----\n");
    pci_enum();
}