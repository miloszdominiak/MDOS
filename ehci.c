#include <ehci.h>

void ehci_init(uint8_t bus, uint8_t slot, uint8_t function)
{
    uint8_t eecp = *(uint8_t*)((pci_read(bus, slot, function, 0x10, 4) & ~0xF) + 9);
    uint32_t capability = pci_read(bus, slot, function, eecp, 4);

    printf("EHCI legacy before: %4", capability);

    pci_write(bus, slot, function, eecp, capability | (1 << 24));

    uint32_t timeout = 100000;
    while(timeout--)
    {
        capability = pci_read(bus, slot, function, eecp, 4);
        if((capability >> 24) & 0x1 && !((capability >> 16) & 0x1)) 
            break;
    }

    *(uint32_t*)(eecp + 4) = 0;
    printf(", after: %4\n", capability);
}