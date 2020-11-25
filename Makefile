CC=/home/mdominiak/opt/cross/bin/i686-elf-gcc
AS=/home/mdominiak/opt/cross/bin/i686-elf-as
CFLAGS=-ffreestanding -Iinclude

myos: out/kernel.o out/terminal.o out/boot.o out/ports.o out/string.o out/pci.o out/stdio.o out/stdlib.o out/uhci.o out/ehci.o out/asmterm.o
	$(CC) -T linker.ld -o myos.bin -ffreestanding -O2 -nostdlib out/boot.o out/kernel.o \
		out/terminal.o out/ports.o out/string.o out/pci.o out/stdio.o out/stdlib.o out/uhci.o out/ehci.o out/asmterm.o -lgcc
	mv myos.bin isodir/boot/myos.bin
	grub-mkrescue -o out/myos.iso isodir
	qemu-system-i386 -cdrom out/myos.iso -usb -device pci-ohci -device usb-ehci -device qemu-xhci

out/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

out/%.o: %.s
	$(AS) $< -o $@

install:
	sudo dd if=out/myos.iso of=/dev/sdc