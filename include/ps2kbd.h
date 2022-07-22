#ifndef PS2KBD_H
#define PS2KBD_H

#include <stdint.h>

#define PS2_KBD_DATA 0x60
#define PS2_KBD_INTERRUPT 1

#define PS2_KBD_ENABLE_SCANNING 0xF4
#define PS2_KBD_SET_TYPEMATIC 0xF3

#define PS2_KBD_ACKNOWLEDGE 0xFA

#define PS2_KBD_DELAY_250MS 0
#define PS2_KBD_DELAY_500MS 1
#define PS2_KBD_DELAY_750MS 2
#define PS2_KBD_DELAY_1000MS 3
#define PS2_KBD_RATE_MAX 0
#define PS2_KBD_RATE_MIN 0x1F

void ps2_keyboard_init();
void scancode_translator();
uint8_t keyboard_read();

#endif