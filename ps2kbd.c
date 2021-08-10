#include <ps2kbd.h>
#include <pic.h>
#include <circular.h>
#include <ports.h>
#include <irq.h>
#include <ps2.h>
#include <stdio.h>

struct Circular scancode_buffer;

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

static void keyboard_interrupt()
{
    circular_push(&scancode_buffer, inb(PS2_KBD_DATA));
    pic_master_eoi();
}

static uint8_t scancode_read()
{
    while(circular_empty(&scancode_buffer))
        asm("hlt");

    return circular_pop(&scancode_buffer);
}

uint8_t set1_to_keycode[256] = { 0x00, 0x01, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26,
                               0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x40,
                               0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 
                               0x49, 0x4A, 0x4B, 0x4C, 0x6C, 0xA0, 0x61, 0x62, 
                               0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 
                               0x6B, 0x20, 0x80, 0x8C, 0x81, 0x82, 0x83, 0x84, 
                               0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x33, 
                               0xA2, 0xA3, 0x60, 0x02, 0x03, 0x04, 0x05, 0x06, 
                               0x07, 0x08, 0x09, 0x0A, 0x0B, 0x31, 0x0F, 0x50, 
                               0x51, 0x52, 0x34, 0x6D, 0x6E, 0x6F, 0x53, 0x8E, 
                               0x8F, 0x90, 0xAB, 0xAC, 0x00, 0x00, 0x00, 0x0C, 
                               0x0D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                               0x00, 0x01, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 
                               0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x40, 
                               0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 
                               0x49, 0x4A, 0x4B, 0x4C, 0x6C, 0xA0, 0x61, 0x62, 
                               0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 
                               0x6B, 0x20, 0x80, 0x8C, 0x81, 0x82, 0x83, 0x84, 
                               0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x33, 
                               0xA2, 0xA3, 0x60, 0x02, 0x03, 0x04, 0x05, 0x06, 
                               0x07, 0x08, 0x09, 0x0A, 0x0B, 0x31, 0x0F, 0x50, 
                               0x51, 0x52, 0x34, 0x6D, 0x6E, 0x6F, 0x53, 0x8E, 
                               0x8F, 0x90, 0xAB, 0xAC, 0x00, 0x00, 0x00, 0x0C,
                               0x0D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

uint8_t set1_pressed[256] = { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
                              1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
                              1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
                              1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
                              1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
                              1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 
                              0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
                              0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
                              0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
                              0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
                              0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
                              0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
                              0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
                              0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

uint8_t set1_to_keycode_extended[256] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                          0x00, 0x00, 0x00, 0x00, 0x91, 0xA7, 0x00, 0x00, 
                                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                          0x00, 0x00, 0x00, 0x00, 0x00, 0x32, 0x00, 0x00, 
                                          0xA4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2F, 
                                          0x8D, 0x30, 0x00, 0xA8, 0x00, 0xAA, 0x00, 0x4E, 
                                          0xA9, 0x4F, 0x2E, 0x4D, 0x00, 0x00, 0x00, 0x00, 
                                          0x00, 0x00, 0x00, 0xA1, 0xA5, 0xA6, 0x13, 0x11, 
                                          0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x00, 
                                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                          0x00, 0x00, 0x00, 0x00, 0x91, 0xA7, 0x00, 0x00, 
                                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                          0x00, 0x00, 0x00, 0x00, 0x00, 0x32, 0x00, 0x00, 
                                          0xA4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2F, 
                                          0x8D, 0x30, 0x00, 0xA8, 0x00, 0xAA, 0x00, 0x4E, 
                                          0xA9, 0x4F, 0x2E, 0x4D, 0x00, 0x00, 0x00, 0x00, 
                                          0x00, 0x00, 0x00, 0xA1, 0xA5, 0xA6, 0x13, 0x11, 
                                          0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x00 };

uint8_t set1_pressed_extended[256] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 
                                       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
                                       0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
                                       0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 
                                       1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 
                                       0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
                                       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
                                       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
                                       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
                                       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
                                       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
                                       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
                                       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
                                       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
 
static void scancode_translator()
{
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
                    printf("0E ");
                }
                else if(scancode == 0xB7 && scancode_read() == 0xE0 && scancode_read() == 0xAA)
                {
                    // printscreen released
                }
                else
                {
                    if(set1_pressed_extended[scancode])
                        printf("%1 ", set1_to_keycode_extended[scancode]);
                }
            }
            else if(scancode == 0xE1 && scancode_read() == 0x1D && scancode_read() == 0x45)
            {
                printf("10 ");
            }
            else if(scancode == 0xE1 && scancode_read() == 0x9D && scancode_read() == 0xC5)
            {
                // pausebreak released
            }
            else
            {
                if(set1_pressed[scancode])
                    printf("%1 ", set1_to_keycode[scancode]);
            }
        }
        asm("hlt");
    }
}

void ps2_keyboard_init()
{
    irq_install_handler(PS2_KBD_INTERRUPT, keyboard_interrupt);
    circular_clear(&scancode_buffer);

    keyboard_send(PS2_KBD_SET_TYPEMATIC);
    keyboard_send(typematic_byte(PS2_KBD_RATE_MAX, PS2_KBD_DELAY_500MS));

    keyboard_send(PS2_KBD_ENABLE_SCANNING);

    scancode_translator();
}