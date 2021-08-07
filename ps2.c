#include <ps2.h>
#include <pci.h>
#include <acpi.h>
#include <stdio.h>
#include <ports.h>
#include <interrupts.h>
#include <pic.h>
#include <circular.h>

struct Circular keyboard_buffer;

static uint8_t ps2_controller_status()
{
    return inb(PS2_STATUS);
}

static void wait_for_write()
{
    while(ps2_controller_status() & PS2_CTRL_INPUT_FULL)
        ;
}

static uint8_t wait_for_read()
{
    int timeout = 10000;
    while(!(ps2_controller_status() & PS2_CTRL_OUTPUT_FULL) && timeout)
        timeout--;

    return timeout;
}

static void ps2_controller_send(uint8_t command)
{
    wait_for_write();
    outb(PS2_COMMAND, command);
}

static void disable_devices()
{
    ps2_controller_send(PS2_CTRL_DISABLE_FIRST);
    ps2_controller_send(PS2_CTRL_DISABLE_SECOND);
}

static uint8_t ps2_controller_read()
{
    if(wait_for_read())
        return inb(PS2_DATA);

    return 0;
}


static void flush_buffer()
{
    while(ps2_controller_read())
        ;
}

static void ps2_data_write(uint8_t data)
{
    wait_for_write();
    outb(PS2_DATA, data);
}

static uint8_t ps2_config_read()
{
    ps2_controller_send(PS2_CONFIG_BYTE_READ);
    return ps2_controller_read();
}

static void ps2_config_write(uint8_t config)
{
    ps2_controller_send(PS2_CONFIG_BYTE_WRITE);
    ps2_data_write(config);
}

static uint8_t ps2_controller_test()
{
    ps2_controller_send(PS2_CTRL_SELFTEST);
    return ps2_controller_read() == PS2_TEST_SUCCESS;
}

static uint8_t ps2_first_test()
{
    ps2_controller_send(PS2_FIRST_TEST);
    return !ps2_controller_read();
}

static void ps2_first_enable()
{
    ps2_controller_send(PS2_CTRL_ENABLE_FIRST);
}

void (*isr_pointer[256])();

void ps2_controller_init()
{
    pci_enum();

    if(!is_ps2_present())
        return;

    printf("PS2 controller present\n");

    disable_devices();
    flush_buffer();

    uint8_t configuration = ps2_config_read();
    configuration &= ~(PS2_CONFIG_FIRST_INT_ENABLED | PS2_CONFIG_SECOND_INT_ENABLED);

    ps2_config_write(configuration);

    if(!ps2_controller_test())
        return;

    ps2_config_write(configuration);
   
    if(!ps2_first_test())
        return;


    configuration |= PS2_CONFIG_FIRST_INT_ENABLED;
    ps2_config_write(configuration);

    ps2_first_enable();

    interrupt_install_handler(1, isr_pointer[178]);
    circular_clear(&keyboard_buffer);

    ps2_data_write(0xFF);

    while(circular_empty(&keyboard_buffer))
        asm("hlt");

    //if(circular_pop(&keyboard_buffer) != 0xFA)
    //;//    return;

    while(circular_empty(&keyboard_buffer))
        asm("hlt");

    circular_clear(&keyboard_buffer);

    printf("PS/2 init success\n");
    ps2_data_write(0xF5);

    while(circular_empty(&keyboard_buffer))
        asm("hlt");

    //if(circular_pop(&keyboard_buffer) != 0xFA)
    //;//    return;

    ps2_data_write(0xF3);

    while(circular_empty(&keyboard_buffer))
        asm("hlt");

    printf("%1\n", circular_pop(&keyboard_buffer));

    ps2_data_write(0x20);

    while(circular_empty(&keyboard_buffer))
        asm("hlt");

    printf("%1\n", circular_pop(&keyboard_buffer));

    printf("tu nie powinienem byc\n");

    ps2_data_write(0xF4);

    printf("Podobno spowolnilem\n");
}

void ps2_keyboard_interrupt()
{
    circular_push(&keyboard_buffer, inb(PS2_DATA));
    pic_master_eoi();
}