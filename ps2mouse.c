#include <ps2mouse.h>
#include <ps2kbd.h>
#include <ps2.h>
#include <circular.h>
#include <stdio.h>
#include <graphics.h>
#include <ports.h>
#include <pic.h>
#include <irq.h>

struct Circular mouse_buffer;

#define CURSIZE 20

int mouse_x = 0, mouse_y = 0;

void czerwony()
{
    size_t wskaznik = mouse_y * screen_width + mouse_x;

    screen_buffer[wskaznik] = 0x00;
    wskaznik += screen_width;
    for(int i = 0; i < 2; i++)
        screen_buffer[wskaznik + i] = 0x00;
    wskaznik += screen_width;
    screen_buffer[wskaznik] = 0x00;
    screen_buffer[wskaznik + 1] = 0xFFFFFF;
    screen_buffer[wskaznik + 2] = 0x00;
    wskaznik += screen_width;
    screen_buffer[wskaznik] = 0x00;
    for(int i = 0; i < 2; i++)
        screen_buffer[wskaznik + 1 + i] = 0xFFFFFF;
    screen_buffer[wskaznik + 3] = 0x00;
    wskaznik += screen_width;
    screen_buffer[wskaznik] = 0x00;
    for(int i = 0; i < 3; i++)
        screen_buffer[wskaznik + 1 + i] = 0xFFFFFF;
    screen_buffer[wskaznik + 4] = 0x00;
    wskaznik += screen_width;
    screen_buffer[wskaznik] = 0x00;
    for(int i = 0; i < 4; i++)
        screen_buffer[wskaznik + 1 + i] = 0xFFFFFF;
    screen_buffer[wskaznik + 5] = 0x00;
    wskaznik += screen_width;
    screen_buffer[wskaznik] = 0x00;
    for(int i = 0; i < 5; i++)
        screen_buffer[wskaznik + 1 + i] = 0xFFFFFF;
    screen_buffer[wskaznik + 6] = 0x00;
    wskaznik += screen_width;
    screen_buffer[wskaznik] = 0x00;
    for(int i = 0; i < 6; i++)
        screen_buffer[wskaznik + 1 + i] = 0xFFFFFF;
    screen_buffer[wskaznik + 7] = 0x00;
    wskaznik += screen_width;
    screen_buffer[wskaznik] = 0x00;
    for(int i = 0; i < 7; i++)
        screen_buffer[wskaznik + 1 + i] = 0xFFFFFF;
    screen_buffer[wskaznik + 8] = 0x00;
    wskaznik += screen_width;
    screen_buffer[wskaznik] = 0x00;
    for(int i = 0; i < 8; i++)
        screen_buffer[wskaznik + 1 + i] = 0xFFFFFF;
    screen_buffer[wskaznik + 9] = 0x00;

    wskaznik += screen_width;
    screen_buffer[wskaznik] = 0x00;
    for(int i = 0; i < 5; i++)
    screen_buffer[wskaznik + 1 + i] = 0xFFFFFF;
    for(int i = 0; i < 5; i++)
    screen_buffer[wskaznik + 6 + i] = 0x00;

    wskaznik += screen_width;
    screen_buffer[wskaznik] = 0x00;
    for(int i = 0; i < 2; i++)
        screen_buffer[wskaznik + 1 + i] = 0xFFFFFF;
    screen_buffer[wskaznik + 3] = 0x00;
    for(int i = 0; i < 2; i++)
        screen_buffer[wskaznik + 4 + i] = 0xFFFFFF;
    screen_buffer[wskaznik + 6] = 0x00;

    wskaznik += screen_width;
    screen_buffer[wskaznik] = 0x00;
    screen_buffer[wskaznik + 1] = 0xFFFFFF;
    screen_buffer[wskaznik + 2] = 0x00;
    screen_buffer[wskaznik + 4] = 0x00;
    for(int i = 0; i < 2; i++)
        screen_buffer[wskaznik + 5 + i] = 0xFFFFFF;
    screen_buffer[wskaznik + 7] = 0x00;

    wskaznik += screen_width;
    for(int i = 0; i < 2; i++)
        screen_buffer[wskaznik + i] = 0x00;
    screen_buffer[wskaznik + 4] = 0x00;
    for(int i = 0; i < 2; i++)
        screen_buffer[wskaznik + 5 + i] = 0xFFFFFF;
    screen_buffer[wskaznik + 7] = 0x00;

    wskaznik += screen_width;
    screen_buffer[wskaznik] = 0x00;
    screen_buffer[wskaznik + 5] = 0x00;
    for(int i = 0; i < 2; i++)
        screen_buffer[wskaznik + 6 + i] = 0xFFFFFF;
    screen_buffer[wskaznik + 8] = 0x00;

    wskaznik += screen_width;
    screen_buffer[wskaznik + 5] = 0x00;
    for(int i = 0; i < 2; i++)
        screen_buffer[wskaznik + 6 + i] = 0xFFFFFF;
    screen_buffer[wskaznik + 8] = 0x00;

    wskaznik += screen_width;
    screen_buffer[wskaznik + 6] = 0x00;
    for(int i = 0; i < 2; i++)
        screen_buffer[wskaznik + 7 + i] = 0xFFFFFF;
    screen_buffer[wskaznik + 9] = 0x00;
    wskaznik += screen_width;
    screen_buffer[wskaznik + 6] = 0x00;
    for(int i = 0; i < 2; i++)
        screen_buffer[wskaznik + 7 + i] = 0xFFFFFF;
    screen_buffer[wskaznik + 9] = 0x00;

    wskaznik += screen_width;
    for(int i = 0; i < 2; i++)
        screen_buffer[wskaznik + 7 + i] = 0x00;
    
}

void czarny()
{
    update_rect(mouse_x, mouse_y, CURSIZE, CURSIZE);
}
void mouse_interrupt()
{
    uint8_t byte = inb(PS2_MOUSE_DATA);
    //printf("Mysz");
    circular_push(&mouse_buffer, byte);
    if(mouse_buffer.size == 3)
    {
        czarny();
        uint8_t zero = circular_pop(&mouse_buffer),
        first = circular_pop(&mouse_buffer),
        second = circular_pop(&mouse_buffer);

        uint8_t state = zero;
	int d = first;
	int rel_x = d - ((state << 4) & 0x100);
	d = second;
	int rel_y = d - ((state << 3) & 0x100);

    mouse_x+= rel_x;
    mouse_y -= rel_y;

    if(mouse_x < 0)
        mouse_x = 0;

    int g = screen_width - 1;
    if(mouse_x > g)
        mouse_x = g;

    if(mouse_y < 0)
        mouse_y = 0;

    if((size_t)mouse_y > screen_height - 1)
        mouse_y = screen_height - 1;

        //printf("YO: %1 XO: %1", (zero & 0x80) == 0x80, (zero & 0x40) == 0x40);
        //printf("%4 %4 %4", zero, first, second);
        //printf("%4 %4", mouse_x, mouse_y);
        //screen_buffer[mouse_y * screen_width + mouse_x] = 0x00FF00;
        //printf("\n");
    czerwony();
    }
    pic_slave_eoi();
    pic_master_eoi();
}


void ps2_mouse_init()
{
    irq_install_handler(PS2_MOUSE_IRQ, mouse_interrupt);
    circular_clear(&mouse_buffer);

    ps2_second_send(PS2_DEVICE_ENABLE_SCANNING);
    circular_clear(&mouse_buffer);
}