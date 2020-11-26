#include <ehci.h>

void ehci_init(uint8_t bus, uint8_t slot, uint8_t function)
{
    uint8_t eecp = *(uint8_t*)(pci_read(bus, slot, function, 0x10, 4) + 9);

    uint32_t legacy = pci_read(bus, slot, function, eecp, 4);
    pci_write(bus, slot, function, eecp, legacy | (1 << 24));
    for(int i = 0; i < 1000000000; i++)
        i++;
}