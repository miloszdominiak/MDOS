#include <ps2kbd.h>
#include <pic.h>
#include <circular.h>
#include <ports.h>
#include <irq.h>
#include <ps2.h>
#include <graphics.h>
#include <keymaps.h>
#include <keypress.h>
#include <threads.h>
#include <terminal.h>
#include <stdio.h>

struct Circular scancode_buffer;
struct KeypressBuffer keypress_buffer;

struct ThreadInfo* keyboard_thread;

uint8_t keyboard_read()
{
    while(circular_empty(&scancode_buffer))
        asm("hlt");

    return circular_pop(&scancode_buffer);
}

static uint8_t keyboard_send(uint8_t command)
{
    printf("Zaczalem czekac");
    if(ps2_wait_for_write())
    {
        outb(PS2_KBD_DATA, command);
        printf("flacezbo");
        return keyboard_read() == PS2_KBD_ACKNOWLEDGE;
    }
    return 0;
}

// static uint8_t typematic_byte(uint8_t rate, uint8_t delay)
// {
//     return (delay << 5) | rate;
// }

#include <stdio.h>
static void keyboard_interrupt()
{
    //update_rect(0, 0, screen_width, screen_height);
    uint8_t byte = inb(PS2_KBD_DATA);
    printf("o");
    printf("%1", byte);
    circular_push(&scancode_buffer, byte);
    printf("d");
    pic_master_eoi();
    printf("byt");
    //thread_unblock(keyboard_thread);
}

static uint8_t scancode_read()
{
    while(circular_empty(&scancode_buffer))
        asm("hlt");

    return circular_pop(&scancode_buffer);
}

uint8_t key_state[256];

uint8_t nie_buforuj;
uint8_t echo;

void keycode_translator(uint8_t keycode)
{
    struct Keypress keypress;
    keypress.keycode = keycode;
    keypress.shifted = 0;
    keypress.alted = 0;

    //printf("%1", keycode);

    if((key_state[0x80] || key_state[0x8B]) && (key_state[0xA4]))
    {
        keypress.ascii = keycode_to_ascii_alted_shifted[keycode];
        keypress.shifted = 1;
        keypress.alted = 1;
    }
    else if(key_state[0x80] || key_state[0x8B])
    {
        keypress.ascii = keycode_to_ascii_shifted[keycode];
        keypress.shifted = 1;
    }
    else if(key_state[0xA4])
    {
        keypress.ascii = keycode_to_ascii_alted[keycode];
        keypress.alted = 1;
    }
    else if(key_state[0xA0] || key_state[0xA7])
    {
        keypress.ascii = keycode_to_ascii_control[keycode];
        keypress.alted = 1;
        keypress.control = 1;
    }
    else 
    {
        keypress.ascii = keycode_to_ascii[keycode];
        keypress.shifted = 0;
    }

    if(key_state[keycode])
    {
        keypress.pressed = 1;
        if(echo)
            putc(keypress.ascii);
        update_cursor();
    }
    else
        keypress.pressed = 0;
    if(!nie_buforuj)
        keypress_push(&keypress_buffer, keypress);
}
 
void scancode_translator()
{
    scheduler_unlock();
    while(1)
    {
        while(!circular_empty(&scancode_buffer))
        {
            uint8_t scancode = scancode_read();
            if(scancode == 0xE0)
            {
                scancode = scancode_read();
                if(scancode == 0x2A && scancode_read() == 0xE0 && scancode_read() == 0x37)
                {
                    key_state[0x0E] = 1;
                    keycode_translator(0x0E);
                }
                else if(scancode == 0xB7 && scancode_read() == 0xE0 && scancode_read() == 0xAA)
                {
                    key_state[0x0E] = 0;
                    keycode_translator(0x0E);
                }
                else
                {
                    if(set1_pressed_extended[scancode])
                        key_state[set1_to_keycode_extended[scancode]] = 1;
                    else
                        key_state[set1_to_keycode_extended[scancode]] = 0;
                    keycode_translator(set1_to_keycode_extended[scancode]);
                }
            }
            else if(scancode == 0xE1 && scancode_read() == 0x1D && scancode_read() == 0x45)
            {
                key_state[0x10] = 1;
                keycode_translator(0x10);
            }
            else if(scancode == 0xE1 && scancode_read() == 0x9D && scancode_read() == 0xC5)
            {
                key_state[0x10] = 0;
                keycode_translator(0x10);
            }
            else
            {
                if(set1_pressed[scancode])
                    key_state[set1_to_keycode[scancode]] = 1;
                else
                    key_state[set1_to_keycode[scancode]] = 0;
                keycode_translator(set1_to_keycode[scancode]);
            }
        }
        thread_block();
    }
}

void ps2_keyboard_init()
{
    irq_install_handler(PS2_KBD_INTERRUPT, keyboard_interrupt);
    circular_clear(&scancode_buffer);

    printf("zaczalem keybo");

    //keyboard_send(PS2_KBD_SET_TYPEMATIC);
    //keyboard_send(typematic_byte(PS2_KBD_RATE_MAX, PS2_KBD_DELAY_500MS));

    keyboard_send(PS2_KBD_ENABLE_SCANNING);

    printf("skonczylem keybo");

    // keyboard_thread = thread_create(scancode_translator, "Keyboard driver");
    // scheduler_lock();
    // schedule();
    // scheduler_unlock();
}