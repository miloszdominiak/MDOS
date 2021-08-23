#include <ps2kbd.h>
#include <pic.h>
#include <circular.h>
#include <ports.h>
#include <irq.h>
#include <ps2.h>
#include <keymaps.h>
#include <keypress.h>
#include <threads.h>
#include <terminal.h>

struct Circular scancode_buffer;
struct KeypressBuffer keypress_buffer;

struct ThreadInfo* keyboard_thread;

static uint8_t keyboard_read()
{
    while(circular_empty(&scancode_buffer))
        asm("hlt");

    return circular_pop(&scancode_buffer);
}

static uint8_t keyboard_send(uint8_t command)
{
    if(ps2_wait_for_write())
    {
        outb(PS2_KBD_DATA, command);
        return keyboard_read() == PS2_KBD_ACKNOWLEDGE;
    }

    return 0;
}

static uint8_t typematic_byte(uint8_t rate, uint8_t delay)
{
    return (delay << 5) | rate;
}

#include <stdio.h>
static void keyboard_interrupt()
{
    circular_push(&scancode_buffer, inb(PS2_KBD_DATA));
    thread_unblock(keyboard_thread);
    pic_master_eoi();
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

    if(key_state[0x80] || key_state[0x8B])
    {
        keypress.ascii = keycode_to_ascii_shifted[keycode];
        keypress.shifted = 1;
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

    keyboard_send(PS2_KBD_SET_TYPEMATIC);
    keyboard_send(typematic_byte(PS2_KBD_RATE_MAX, PS2_KBD_DELAY_500MS));

    keyboard_send(PS2_KBD_ENABLE_SCANNING);

    keyboard_thread = thread_create(scancode_translator);
    scheduler_lock();
    schedule();
    scheduler_unlock();
}