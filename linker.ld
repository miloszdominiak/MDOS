ENTRY(_start)

SECTIONS
{
    . = 1M;

    .text :
    {
        *(.multiboot)
        *(.text)
    }

    


}