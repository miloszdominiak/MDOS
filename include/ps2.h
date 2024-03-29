#ifndef PS_2
#define PS_2

#include <stdint.h>
#include <stdbool.h>

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
#define PS2_CTRL_ENABLE_FIRST 0xAE
#define PS2_CTRL_ENABLE_SECOND 0xA8
#define PS2_CONFIG_BYTE_READ 0x20
#define PS2_CONFIG_BYTE_WRITE 0x60
#define PS2_CTRL_SELFTEST 0xAA
#define PS2_FIRST_TEST 0xAB
#define PS2_SECOND_TEST 0xA9

#define PS2_CTRL_TEST_SUCCESS 0x55

#define PS2_DEVICE_RESET 0xFF
#define PS2_DEVICE_DISABLE_SCANNING 0xF5
#define PS2_DEVICE_ENABLE_SCANNING 0xF4
#define PS2_DEVICE_IDENTIFY 0xF2

#define PS2_DEVICE_TEST_PASSED 0xAA
#define PS2_DEVICE_ACKNOWLEDGE 0xFA

void ps2_controller_init();
bool ps2_wait_for_write();
uint8_t ps2_second_send(uint8_t command);

#endif