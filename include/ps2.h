#ifndef PS_2
#define PS_2

#include <stdint.h>
#include <circular.h>

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
#define PS2_CTRL_DISABLE_SECOND 0xA8
#define PS2_CTRL_ENABLE_FIRST 0xAE
#define PS2_CONFIG_BYTE_READ 0x20
#define PS2_CONFIG_BYTE_WRITE 0x60
#define PS2_CTRL_SELFTEST 0xAA
#define PS2_FIRST_TEST 0xAB

#define PS2_TEST_SUCCESS 0x55

extern struct Circular keyboard_buffer;

void ps2_controller_init();

#endif