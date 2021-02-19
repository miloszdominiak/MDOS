#include <terminal.h>
#include <stdio.h>
#include <acpi.h>

void kernel_main()
{
    set_color(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_CYAN);
    terminal_initialize();

    printf("MDOS\n");
    printf("----\n");

    printf("PS2 present: %1\n", is_ps2_present());
}