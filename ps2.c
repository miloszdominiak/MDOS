#include <ps2.h>
#include <stdio.h>
#include <ports.h>
#include <circular.h>
#include <irq.h>
#include <ps2kbd.h>
#include <ps2mouse.h>
#include <stdbool.h>

static uint8_t ps2_controller_status()
{
    return inb(PS2_STATUS);
}

bool ps2_wait_for_write()
{
    int timeout = 500000;
    while(ps2_controller_status() & PS2_CTRL_INPUT_FULL && timeout)
        timeout--;

    return timeout;
}

static uint32_t wait_for_read()
{
    int timeout = 500000;
    while(!(ps2_controller_status() & PS2_CTRL_OUTPUT_FULL) && timeout)
        timeout--;
    return timeout;
}

static uint8_t ps2_controller_send(uint8_t command)
{
    if(ps2_wait_for_write())
    {
        outb(PS2_COMMAND, command);
        return 1;
    }

    return 0;
}

static uint8_t ps2_controller_read()
{
    if(wait_for_read())
        return inb(PS2_DATA);

    return 0;
}

static uint8_t ps2_first_send(uint8_t command)
{
    if(ps2_wait_for_write())
    {
        outb(PS2_DATA, command);
        return (ps2_controller_read() == PS2_DEVICE_ACKNOWLEDGE);
    }

    return 0;
}

uint8_t ps2_second_send(uint8_t command)
{
    outb(PS2_COMMAND, 0xD4);
    return ps2_first_send(command);
}

static void flush_buffer()
{
    int chuj;
    while((chuj = ps2_controller_read()))
        printf("flush %1 ", chuj);
}

static uint8_t ps2_data_write(uint8_t data)
{
    if(ps2_wait_for_write())
    {
        outb(PS2_DATA, data);
        return 1;
    }

    return 0;
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


void ps2_controller_init()
{
    ps2_controller_send(PS2_CTRL_DISABLE_FIRST);
    ps2_controller_send(PS2_CTRL_DISABLE_SECOND);
    
    flush_buffer();

    uint8_t configuration = ps2_config_read();
    configuration &= ~(PS2_CONFIG_FIRST_INT_ENABLED | 
                        PS2_CONFIG_SECOND_INT_ENABLED);
    ps2_config_write(configuration);

    bool single_channel = false;
    if(!(configuration & PS2_CONFIG_SECOND_DISABLED))
    {
        printf("Single channel PS/2 controller\n");
        single_channel = true;
    }

    ps2_controller_send(PS2_CTRL_SELFTEST);
    if(ps2_controller_read() != PS2_CTRL_TEST_SUCCESS)
    {
        printf("PS/2 controller broken (self-test failed)\n");  
        return;
    }

    ps2_config_write(configuration);

    if(!single_channel)
    {
        ps2_controller_send(PS2_CTRL_ENABLE_SECOND);
        configuration = ps2_config_read();
        if(configuration & PS2_CONFIG_SECOND_DISABLED)
        {
            printf("Single channel PS/2 controller\n");
            single_channel = true;
        }

        if(!single_channel)
            ps2_controller_send(PS2_CTRL_DISABLE_SECOND);
    }
   
    bool first_active = true;
    ps2_controller_send(PS2_FIRST_TEST);
    if(ps2_controller_read() != 0x00)
        first_active = false;

    bool second_active = true;
    if(!single_channel)
    {
        ps2_controller_send(PS2_SECOND_TEST);
        if(ps2_controller_read() != 0x00)
            second_active = false;
    }
    else
        second_active = false;

    if(!(first_active || second_active))
    {
        printf("PS/2 controller broken (no ports available)\n");  
        return;
    }

    if(first_active)
        configuration |= PS2_CONFIG_FIRST_INT_ENABLED;

    if(second_active)
    {
        configuration |= PS2_CONFIG_SECOND_INT_ENABLED;
    }
    ps2_config_write(configuration);

    if(first_active)
    {
        ps2_controller_send(PS2_CTRL_ENABLE_FIRST);
        if(ps2_first_send(PS2_DEVICE_RESET))
        {
            if(ps2_controller_read() != PS2_DEVICE_TEST_PASSED)
            {
                printf("First device broken (test failed)\n");
                //return;
            }
        }
        else
            printf("First device disconnected\n");

        if(ps2_first_send(PS2_DEVICE_DISABLE_SCANNING) && 
            ps2_first_send(PS2_DEVICE_IDENTIFY))
        {
            if(wait_for_read())
            {
                uint8_t response = ps2_controller_read();
                if(response == 0x00)
                    printf("Standard PS/2 mouse\n");
                else if(response == 0x03)
                    printf("Mouse with scroll wheel\n");
                else if(response == 0x04)
                    printf("Mouse with 5 buttons\n");
                else if(response == 0xAB)
                {
                    if(ps2_controller_read() == 0x83)
                        printf("MF2 keyboard\n");
                    else
                    {
                        printf("MF2 keyboard with translation\n");
                        //ps2_keyboard_init();
                    }
                }
            }
            else
                printf("Old AT keyboard with translation\n");
        }
    }

    if(second_active)
    {
        ps2_controller_send(PS2_CTRL_ENABLE_SECOND);

        if(ps2_second_send(PS2_DEVICE_RESET))
        {
            if(ps2_controller_read() != PS2_DEVICE_TEST_PASSED)
            {
                printf("Second device broken (test failed)\n");
                //return;
            }
        }

        wait_for_read();
        ps2_controller_read();

        if(ps2_second_send(PS2_DEVICE_DISABLE_SCANNING) && ps2_second_send(PS2_DEVICE_IDENTIFY))
        {
            if(wait_for_read())
            {
                uint8_t response = ps2_controller_read();
                if(response == 0x00)
                {
                    printf("Standard PS/2 mouse\n");

                    ps2_mouse_init();
                }
                else if(response == 0x03)
                    printf("Mouse with scroll wheel\n");
                else if(response == 0x04)
                    printf("Mouse with 5 buttons\n");
                else if(response == 0xAB)
                {
                    if(ps2_controller_read() == 0x83)
                        printf("MF2 keyboard\n");
                    else
                    {
                        printf("MF2 keyboard with translation\n");
                        //ps2_keyboard_init();
                    }
                }
            }
            else
                printf("Old AT keyboard with translation\n");
        }

        //irq_install_handler(12, mouse_interrupt);
        
        //printf("po");
        
    }
}

