#include <ps2.h>
#include <pci.h>
#include <acpi.h>
#include <stdio.h>
#include <ports.h>
#include <interrupts.h>
#include <pic.h>

static void wait_for_write()
{
    while(ps2_controller_status() & PS2_CTRL_INPUT_FULL)
        ;
}

static uint8_t wait_for_read()
{
    int timeout = 1000000;
    while(!(ps2_controller_status() & PS2_CTRL_OUTPUT_FULL) && timeout)
        timeout--;

    return timeout;
}

static void disable_devices()
{
    ps2_controller_send(PS2_CTRL_DISABLE_FIRST);
    ps2_controller_send(PS2_CTRL_DISABLE_SECOND);
}

static void flush_buffer()
{
    uint8_t scancode;
    while((scancode = ps2_controller_read()))
        printf("f%1 \n", scancode);
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

    return 0;
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
    ps2_controller_send(PS2_CTRL_SELFTEST);
    return ps2_controller_read() == PS2_TEST_SUCCESS;
}

uint8_t ps2_first_test()
{
    ps2_controller_send(PS2_FIRST_TEST);
    return !ps2_controller_read();
}

void ps2_controller_init()
{
    pci_enum();

    if(!is_ps2_present())
        return;

    printf("PS2 controller present\n");

    disable_devices();
    
    flush_buffer();

    uint8_t configuration = ps2_config_read();
    printf("Config: %1\n", configuration);

    configuration &= ~(PS2_CONFIG_FIRST_INT_ENABLED | PS2_CONFIG_SECOND_INT_ENABLED);
    printf("Ja chce: %1\n", configuration);
    ps2_config_write(configuration);

    if(!ps2_controller_test())
        return;

    printf("Config: %1\n", configuration);
   
    if(!ps2_first_test())
        return;

    ps2_controller_send(0xAE);
    flush_buffer();

    ps2_data_write(0xFF);
    flush_buffer();


    ps2_data_write(0xF5);
    flush_buffer();

    ps2_data_write(0xF2);
    flush_buffer();

    printf("Tera czas na sterownik klawiaturowy\n");

    
    interrupt_install_handler(1, irq1);

    disable_devices();

    

    configuration |= PS2_CONFIG_FIRST_INT_ENABLED;
    printf("Ja chce: %1\n", configuration);
    ps2_config_write(configuration);

    ps2_controller_send(0xAE);

    ps2_data_write(0xFF);
    flush_buffer();
    //printf("%1", ps2_controller_read());

    //configuration = ps2_config_read();
    //printf("Config: %1\n", configuration);
    
     //ps2_controller_send(0xAE);
    // flush_buffer();

    Chodź w ramiona pierdolona, zatańczymy sobie walca.
    Ja cię klepnę raz po piździe, ty mnie klepniesz raz po jajcach.
    O kurwa, o kurwa, jaki piękny walczyk!
    Po chuju, po chuju mnie się z panią tańczy!
    Ty kurwo, ty kurwo, nie depcz mi po palcach!
    Bo jak ci wyjebię to nie skończysz walca!
}

void ps2_keyboard_interrupt()
{
    printf("i%1", inb(0x60));
    pic_master_eoi();
}