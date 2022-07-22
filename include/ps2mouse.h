#ifndef PS2MOUSE_H
#define PS2MOUSE_H

#define PS2_MOUSE_DATA 0x60
#define PS2_MOUSE_IRQ 12

#define PS2_MOUSE_ENABLE_SCANNING 0xF4

#define PS2_MOUSE_ACKNOWLEDGE 0xFA

void ps2_mouse_init();

#endif