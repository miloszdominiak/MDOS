#include <uhci.h>

void uhci_init(uint8_t bus, uint8_t slot, uint8_t function)
{
    pci_write(bus, slot, function, 0xC0, 0x00008F00);
}