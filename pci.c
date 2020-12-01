#include <ports.h>
#include <stdio.h>
#include <uhci.h>
#include <ehci.h>
#include <xhci.h>

static uint32_t pci_address(uint8_t bus, uint8_t slot, uint8_t function, uint8_t offset)
{
    return 1 << 31 | bus << 16 | slot << 11 | function << 8 | (offset & 0xFC);
}

uint32_t pci_read(uint8_t bus, uint8_t slot, uint8_t function, uint8_t offset, uint8_t bytes)
{
    outd(0x0CF8, pci_address(bus, slot, function, offset));

    return (ind(0x0CFC) >> (offset & 0x3) * 8) & 0xFFFFFFFF >> (4 - bytes) * 8;
}

void pci_write(uint8_t bus, uint8_t slot, uint8_t function, uint8_t offset, uint32_t data)
{
    outd(0x0CF8, pci_address(bus, slot, function, offset));

    outd(0x0CFC, data);
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
                uint8_t type = pci_read(bus, slot, function, 0x9, 1);

                if(type == 0x00)
                    uhci_init(bus, slot, function);
                else if(type == 0x20)
                    ehci_init(bus, slot, function);
                else if(type == 0x30)
                    xhci_init(bus, slot, function);
            }
        }
    }

    printf("Disabled USB Legacy\n");
}