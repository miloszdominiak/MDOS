#include <terminal.h>
#include <stdio.h>
#include <gdt.h>
#include <idt.h>
#include <pci.h>
#include <threads.h>
#include <vga.h>
#include <ps2.h>

uint32_t rand();

int round(double number)
{
    return number + 0.5;
}

void draw_line(int x1, int y1, int x2, int y2, uint8_t color)
{
    double slope = 0;
    if((x2 - x1) != 0)
        slope = (double)(y2 - y1) / (x2 - x1);

    for(int x = x1; x <= x2; x++)
    {
        int y = round(slope * (x - x1) + y1);
        vga_put_pixel(x, y, color);
    }
}

// char payload[] = {
// 0x03, 0xff, 00, 00, 00, 00, 00, 00, 00, 00, 0x66, 0xfa, 0x20, 0xb8, 00, 00,
// 0x8e, 00, 0x8e, 0xd8, 0x8e, 0xc0, 0x8e, 0xe0, 0x8e, 0xe8, 0xea, 0xd0, 0x00, 0x20, 00, 00,
// 00, 0xbc, 0xb8, 0x80, 00, 00, 0xd8, 0x8e, 0xc0, 0x8e, 0xe0, 0x8e, 0xe8, 0x8e, 0xd0, 0x8e,
// 1, 0x0f, 00, 0x1e, 0xfb, 00, 0x05, 0xb4, 00, 0xb0, 0x10, 0xcd, 0x0a, 0xb4, 0x2f, 0xb0,
// 00, 0xb7, 0x01, 0xb9, 0xcd, 00, 0xf4, 0x10, 0xfd, 0xeb
// };

char payload[] = {0x03, 0xff, 00, 00, 00, 00, 00, 00, 00, 00, 0x66, 0xfa, 0x20, 0xb8, 00, 00,
0x8e, 00, 0x8e, 0xd8, 0x8e, 0xc0, 0x8e, 0xe0, 0x8e, 0xe8, 0xea, 0xd0, 0x00, 0x20, 00, 00,
00, 0xbc, 0xb8, 0x80, 00, 00, 0xd8, 0x8e, 0xc0, 0x8e, 0xe0, 0x8e, 0xe8, 0x8e, 0xd0, 0x8e,
1, 0x0f, 00, 0x1e, 0xf4, 0x00, 0xfd, 0xeb};

extern uint8_t sperma;

void ucieczka();

#define handler(numer)

uint32_t* wskaznik;




uint32_t kolory[1000];

void kernel_main(uint32_t* address)
{
    set_color(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_GREY);
    terminal_initialize(address[22]);

    printf("MDOS\n");
    printf("----\n");

    gdt_init();
    idt_init();
    pci_enum();

    threads_init();

    ps2_controller_init();

    uint8_t dupa;
    while(1)
    {
        scanf("%c", &dupa);
        putc(dupa);
        update_cursor();
    }


    while(1);
}