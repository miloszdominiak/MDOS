#ifndef GDT_H
#define GDT_H

#include <stdint.h>

#define GDT_NULL_SELECTOR 0x0
#define GDT_KERNEL_SELECTOR 0x8
#define GDT_DATA_SELECTOR 0x10

#define GDT_ACCESS_READABLE (1 << 1)
#define GDT_ACCESS_WRITABLE (1 << 1)
#define GDT_ACCESS_GROWS_DOWN (1 << 2)
#define GDT_ACCESS_EXEC_INHERIT (1 << 2)
#define GDT_ACCESS_EXECUTABLE (1 << 3)
#define GDT_ACCESS_CODE_OR_DATA (1 << 4)
#define GDT_ACCESS_RING0 (0 << 5)
#define GDT_ACCESS_RING1 (1 << 5)
#define GDT_ACCESS_RING2 (2 << 5)
#define GDT_ACCESS_RING3 (3 << 5)
#define GDT_ACCESS_PRESENT (1 << 7)

#define GDT_FLAGS_32 (1 << 2)
#define GDT_FLAGS_4K_BLOCK (1 << 3)

#define GDT_KERNEL_ACCESS (GDT_ACCESS_PRESENT | GDT_ACCESS_CODE_OR_DATA | \
    GDT_ACCESS_EXECUTABLE | GDT_ACCESS_READABLE | GDT_ACCESS_RING0)
#define GDT_DATA_ACCESS (GDT_ACCESS_PRESENT | GDT_ACCESS_CODE_OR_DATA | \
    GDT_ACCESS_WRITABLE | GDT_ACCESS_RING0)

struct GDTSelector
{
    uint16_t LimitLower;
    uint16_t BaseLower;
    uint8_t BaseMedium;
    uint8_t AccessByte;
    uint8_t LimitHigher:4;
    uint8_t Flags:4;
    uint8_t BaseHigher;
} __attribute__ ((packed));

struct GDTPointer
{
    uint16_t Size;
    uint32_t Offset;
} __attribute__ ((packed));

void gdt_init();
void load_gdt(struct GDTPointer gdtp);

#endif