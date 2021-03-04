#ifndef PS_2
#define PS_2

#include <stdint.h>

#define PS2_COMMAND 0x64
#define PS2_STATUS 0x64
#define PS2_DATA 0x60

#define PS2_CTRL_OUTPUT_FULL (1 << 0)
#define PS2_CTRL_INPUT_FULL (1 << 1)

#define PS2_CONFIG_FIRST_INT_ENABLED (1 << 0)
#define PS2_CONFIG_SECOND_INT_ENABLED (1 << 1)
#define PS2_CONFIG_FIRST_DISABLED (1 << 4)
#define PS2_CONFIG_SECOND_DISABLED (1 << 5)

#define PS2_CTRL_DISABLE_FIRST 0xAD
#define PS2_CTRL_DISABLE_SECOND 0xA7
#define PS2_CONFIG_BYTE_READ 0x20
#define PS2_CONFIG_BYTE_WRITE 0x60
#define PS2_SELFTEST 0xAA

void ps2_controller_init();
void ps2_controller_send(uint8_t command);
uint8_t ps2_controller_read();
uint8_t ps2_controller_status();

#endif