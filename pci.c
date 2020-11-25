#include <ports.h>
#include <stdio.h>

uint32_t pci_read(uint8_t bus, uint8_t slot, uint8_t function, uint8_t offset, uint8_t bytes)
{
    uint32_t address = 1 << 31 | bus << 16 | slot << 11 | function << 8 | (offset & 0xFC);
    outd(0x0CF8, address);

    uint32_t ret = ind(0x0CFC);
    return (ret >> (offset & 0x03) * 8) & 0xFFFFFFFF >> (4 - bytes) * 8;
}

void pci_write(uint8_t bus, uint8_t slot, uint8_t function, uint8_t offset, uint32_t data)
{
    uint32_t address = 1 << 31 | bus << 16 | slot << 11 | function << 8 | offset;
    outd(0x0CF8, address);

    outd(0x0CFC, data);
}

void pci_write_byte(uint8_t bus, uint8_t slot, uint8_t function, uint8_t offset, uint8_t data)
{
    uint32_t address = 1 << 31 | bus << 16 | slot << 11 | function << 8 | (offset & 0xFC);
    outd(0x0CF8, address);

    outb(0x0CFC + offset & 0x3, data);
}

uint16_t pci_size(uint8_t bus, uint8_t slot, uint8_t function, uint8_t is_uhci)
{
    int command = pci_read(bus, slot, function, 0x4, 4);
    
    pci_write(bus, slot, function, 0x4, command & ~0x0007);

    int base = pci_read(bus, slot, function, is_uhci ? 0x20 : 0x10, 4);

    pci_write(bus, slot, function, is_uhci ? 0x20 : 0x10, 0xFFFFFFFF);

    int prawie_size = pci_read(bus, slot, function, is_uhci ? 0x20 : 0x10, 4);

    pci_write(bus, slot, function, is_uhci ? 0x20 : 0x10, base);

    pci_write(bus, slot, function, 0x4, command);

    return ~(prawie_size & (is_uhci ? ~0x00000001 : ~0x0000000F)) + 1;
}

void pci_enum()
{
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