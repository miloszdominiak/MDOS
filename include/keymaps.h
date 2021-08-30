#ifndef KEYMAPS_H
#define KEYMAPS_H

#include <stdint.h>

extern uint8_t set1_to_keycode[256];
extern uint8_t set1_to_keycode_extended[256];

extern uint8_t set1_pressed[256];
extern uint8_t set1_pressed_extended[256];

extern uint8_t keycode_to_ascii[256];
extern uint8_t keycode_to_ascii_shifted[256];
extern uint8_t keycode_to_ascii_alted[256];
extern uint8_t keycode_to_ascii_alted_shifted[256];

#endif