CC = /home/mdominiak/opt/cross/bin/i686-elf-gcc
AS = /home/mdominiak/opt/cross/bin/i686-elf-as
CFLAGS = -ffreestanding -Iinclude -Wall -Wextra -Werror -g
OBJ = \
out/boot_asm.o \
out/kernel.o \
out/terminal.o \
out/stdio.o \
out/ports_asm.o \
out/gdt.o \
out/gdt_asm.o \
out/idt.o \
out/idt_asm.o \
out/pic.o \
out/pci.o \
out/uhci.o \
out/ehci.o \
out/xhci.o \
out/stdlib.o \
out/irq.o \
out/threads.o \
out/threads_asm.o \
out/ps2.o \
out/ps2kbd.o \
out/circular.o \
out/keypress.o \
out/keymaps.o \
out/string.o \
out/terminal_asm.o \
out/vga.o \
out/rand.o

all: out/myos.bin
	cp out/myos.bin isodir/boot/myos.bin
	grub-mkrescue -o out/myos.iso isodir
	cp out/myos.iso /mnt/c/users/milos/onedrive/pulpit 

	qemu-system-i386 -s -cdrom out/myos.iso -usb -device usb-ehci -device qemu-xhci -vga std

out/myos.bin: $(OBJ)
	$(CC) -T linker.ld -o $@ -ffreestanding -O2 -nostdlib out/*.o -lgcc

out/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

out/%.o: %.s
	$(AS) $< -o $@

install:
	sudo dd if=out/myos.iso of=/dev/sdc
	sync

clean:
	rm out/*.o
	rm out/myos.iso