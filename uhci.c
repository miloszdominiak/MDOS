#include <uhci.h>
#include <stdio.h>
#include <pci.h>

void uhci_init(uint8_t bus, uint8_t slot, uint8_t function)
{
    printf("UHCI legacy before: %2", pci_read(bus, slot, function, 0xC0, 2));
    pci_write(bus, slot, function, 0xC0, 0x00008F00);
    printf(", after: %2\n", pci_read(bus, slot, function, 0xC0, 2));
}