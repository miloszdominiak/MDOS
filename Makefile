CC=/home/mdominiak/opt/cross/bin/i686-elf-gcc
AS=/home/mdominiak/opt/cross/bin/i686-elf-as
CFLAGS=-ffreestanding -Iinclude -Wall -Wextra -Werror

myos: out/kernel.o out/terminal.o out/boot.o out/ports.o out/string.o \
		out/pci.o out/stdio.o out/stdlib.o out/uhci.o out/ehci.o out/xhci.o \
		out/acpi.o out/asmterm.o out/ps2.o out/idt.o out/gdt.o out/gdt_asm.o \
		out/pic.o out/idt_asm.o out/circular.o out/irq.o out/ps2kbd.o out/keymaps.o \
		out/keypress.o
	$(CC) -T linker.ld -o myos.bin -ffreestanding -O2 -nostdlib out/*.o -lgcc
	mv myos.bin isodir/boot/myos.bin
	grub-mkrescue -o out/myos.iso isodir
	qemu-system-i386 -cdrom out/myos.iso -usb -device usb-ehci -device qemu-xhci
	cp out/myos.iso /mnt/c/users/milos/onedrive/pulpit

out/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

out/%.o: %.s
	$(AS) $< -o $@

install:
	sudo dd if=out/myos.iso of=/dev/sdc
	sync

clean:
	rm out/*.o