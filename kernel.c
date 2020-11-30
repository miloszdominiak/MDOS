#include <terminal.h>
#include <stdio.h>

void kernel_main()
{
    set_color(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_MAGENTA);
    terminal_initialize();

    printf("MDOS\n");
    printf("----\n");

    pci_enum();

    if(is_ps2())
    {
        printf("PS/2 controller present\n");
    }
    else
    {
        printf("No PS/2 controller\n");
    }

    for(int i = 0; i < 100000000; i++)
        i++;

    
    outb(0x64, 0xAD);
    for(int i = 0; i < 100000000; i++)
        i++;
    outb(0x64, 0xA7);
    for(int i = 0; i < 100000000; i++)
        i++;
    printf("Disabled devices\n");

    inb(0x60);
    printf("Flushed\n");

    for(int i = 0; i < 100000000; i++)
        i++;
    outb(0x64, 0x20);
    for(int i = 0; i < 100000000; i++)
        i++;
    uint8_t configuration = inb(0x60);
    for(int i = 0; i < 100000000; i++)
        i++;
    printf("Configuration: %1\n", configuration);

    configuration &= ~0x03;
    printf("Our configuration: %1\n", configuration);
    for(int i = 0; i < 100000000; i++)
        i++;
    outb(0x64, 0x60);
    for(int i = 0; i < 100000000; i++)
        i++;
    outb(0x60, configuration);

    for(int i = 0; i < 100000000; i++)
        i++;
    outb(0x64, 0x20);
    for(int i = 0; i < 100000000; i++)
        i++;
    printf("Configuration: %1\n", inb(0x60));

    for(int i = 0; i < 100000000; i++)
        i++;
    outb(0x64, 0xAA);
    for(int i = 0; i < 100000000; i++)
        i++;
    printf("Self test: %1\n", inb(0x60));

    for(int i = 0; i < 100000000; i++)
        i++;
    outb(0x64, 0xA8);
    printf("Enabled second port\n");

    for(int i = 0; i < 100000000; i++)
        i++;
    outb(0x64, 0x20);
    for(int i = 0; i < 100000000; i++)
        i++;
    printf("Configuration: %1\n", inb(0x60));

    for(int i = 0; i < 100000000; i++)
        i++;
    outb(0x64, 0xA7);

    for(int i = 0; i < 100000000; i++)
        i++;
    outb(0x64, 0xAB);
    for(int i = 0; i < 100000000; i++)
        i++;
    printf("First port test: %1\n", inb(0x60));

    for(int i = 0; i < 100000000; i++)
        i++;
    outb(0x64, 0xA9);
    for(int i = 0; i < 100000000; i++)
        i++;
    printf("Second port test: %1\n", inb(0x60));

    for(int i = 0; i < 100000000; i++)
        i++;
    outb(0x64, 0xAE);
    for(int i = 0; i < 100000000; i++)
        i++;
    outb(0x64, 0xA8);
    printf("Enabled devices\n");

    for(int i = 0; i < 100000000; i++)
        i++;
    inb(0x60);
    for(int i = 0; i < 100000000; i++)
        i++;
    outb(0x60, 0xFF);
    for(int i = 0; i < 100000000; i++)
        i++;
    printf("First port reset: %1\n", inb(0x60));
    for(int i = 0; i < 100000000; i++)
        i++;
    printf("First port reset: %1\n", inb(0x60));

    for(int i = 0; i < 100000000; i++)
        i++;
    outb(0x64, 0xD4);
    for(int i = 0; i < 100000000; i++)
        i++;
    outb(0x60, 0xFF);
    for(int i = 0; i < 100000000; i++)
        i++;
    printf("Second port reset: %1\n", inb(0x60));

    for(int i = 0; i < 100000000; i++)
        i++;
    printf("%1 ", inb(0x60));

    for(int i = 0; i < 100000000; i++)
        i++;
    outb(0x60, 0xF4);
    for(int i = 0; i < 100000000; i++)
        i++;
    outb(0x64, 0xD4);
    for(int i = 0; i < 100000000; i++)
        i++;
    outb(0x60, 0xF4);

    for(int i = 0; i < 100000000; i++)
        i++;
    

    for(int i = 0; i < 100000000; i++)
        i++;
    outb(0x60, 0xF0);
    for(int i = 0; i < 100000000; i++)
        i++;
    outb(0x60, 2);

    for(int i = 0; i <= 100; i++)
    {
        if(inb(0x64) & 1)
        printf("%1 ", inb(0x60));
    }

    for(int i = 0; i < 100000000; i++)
        i++;
    outb(0x60, 0xF0);
    for(int i = 0; i < 100000000; i++)
        i++;
    outb(0x60, 0);


    for(int i = 0; i < 100000000; i++)
        i++;

        for(int i = 0; i <= 100; i++)
    {
        if(inb(0x64) & 1)
        printf("%1 ", inb(0x60));
    }

            printf("\n");
    while(1)
    {
        if(inb(0x64) & 1)
        printf("%1 ", inb(0x60));
    }
}