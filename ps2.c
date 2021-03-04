#include <ps2.h>
#include <pci.h>
#include <acpi.h>
#include <stdio.h>
#include <ports.h>

inline static void wait_for_write()
{
    while(ps2_controller_status() & PS2_CTRL_INPUT_FULL)
        ;
}

static uint8_t wait_for_read()
{
    int timeout = 10000000;
    while(!(ps2_controller_status() & PS2_CTRL_OUTPUT_FULL) && timeout)
        timeout--;

    return timeout;
}

static void disable_devices()
{
    ps2_controller_send(PS2_CTRL_DISABLE_FIRST);
    ps2_controller_send(PS2_CTRL_DISABLE_SECOND);
}

void flush_buffer()
{
    int zyd;
    while((zyd = ps2_controller_read()) != 0xFF)
        printf("%1 \n", zyd);
}

uint8_t ps2_controller_status()
{
    return inb(PS2_STATUS);
}

void ps2_controller_send(uint8_t command)
{
    wait_for_write();
    outb(PS2_COMMAND, command);
}

void ps2_data_write(uint8_t data)
{
    wait_for_write();
    outb(PS2_DATA, data);
}

uint8_t ps2_controller_read()
{
    if(wait_for_read())
        return inb(PS2_DATA);

    return 0xFF;
}

uint8_t ps2_config_read()
{
    ps2_controller_send(PS2_CONFIG_BYTE_READ);
    return ps2_controller_read();
}

void ps2_config_write(uint8_t config)
{
    ps2_controller_send(PS2_CONFIG_BYTE_WRITE);
    ps2_data_write(config);
}

uint8_t ps2_controller_test()
{
    ps2_controller_send(PS2_SELFTEST);
    return ps2_controller_read();
}

void ps2_controller_init()
{
    pci_enum();

    if(!is_ps2_present())
        return;
    printf("PS2 controller present\n");

    disable_devices();
    printf("Devices disabled\n");

    flush_buffer();

    uint8_t configuration = ps2_config_read();
    printf("Config: %1\n", configuration);

    configuration |= PS2_CONFIG_FIRST_INT_ENABLED;
    printf("Ja chce: %1\n", configuration);
    ps2_config_write(configuration);

    if(ps2_controller_test() != 0x55)
        return;

    printf("Ja chce: %1\n", configuration);
    ps2_config_write(configuration);

    

    configuration = ps2_config_read();
    printf("Config: %1\n", configuration);

    ps2_controller_send(0xAE);

}