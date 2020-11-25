#include <terminal.h>
#include <ports.h>
#include <pci.h>
#include <string.h>
#include <stdio.h>

void kernel_main()
{
    set_color(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_CYAN);
    terminal_initialize();

    char buffer[20];
    printf("Start\n");
    for(uint16_t bus = 0; bus < 256; bus++)
    for(uint8_t slot = 0; slot < 32; slot++)
    for(uint8_t function = 0; function < 8; function++)
    {
        uint16_t vendor = pci_read(bus, slot, function, 0, 2);
        if(vendor != 0xFFFF)
        {
            uint8_t class = pci_read(bus, slot, function, 0xB, 1);
            uint8_t subclass = pci_read(bus, slot, function, 0xA, 1);
            
            if(class == 0xC && subclass == 0x3)
            {
                printf("Znalazlem kontroler USB. ");
                uint8_t type = pci_read(bus, slot, function, 0x9, 1);
                char is_uhci = 0;
                if(type == 0x00)
                {
                    printf("(UHCI)\n");
                    is_uhci = 1;
                }
                if(type == 0x10)
                    printf("(OHCI)\n");

                if(type == 0x20)
                    printf("(EHCI)\n");

                if(type == 0x30)
                    printf("(xHCI)\n");

                uint32_t base_address = pci_read(bus, slot, function, is_uhci ? 0x20 : 0x10, 4);
                base_address &= (is_uhci ? ~0x1 : ~0xF);
                uint16_t size = pci_size(bus, slot, function, is_uhci);
                printf("Bus = 0x%1, device = 0x%1, function = 0x%1, base = 0x%4, size = 0x%2\n",
                    bus, slot, function, base_address, size);

                if(is_uhci)
                {
                    printf("Legacy = %2\n", pci_read(bus, slot, function, 0xC0, 4));
                    pci_write(bus, slot, function, 0xC0, 0x00008F00);
                    printf("Legacy = %2\n", pci_read(bus, slot, function, 0xC0, 4));
                }
                else
                {
                    uint16_t eecp;

                    uint32_t ext;
                    printf("Legacy = 0x%4\n", ext = pci_read(bus, slot, function, eecp, 4));
                    pci_write(bus, slot, function, eecp, ext | (1 << 24));
                    for(int i = 0; i < 1000000000; i++)
                        i++;
                    printf("Legacy = 0x%4\n", pci_read(bus, slot, function, eecp, 4));
                }
            }
        }
    }
}