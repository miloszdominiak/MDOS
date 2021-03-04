.set MAGIC, 0x1BADB002
.set FLAGS, 0
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.section .bss
.align 16
stack_bottom:
    .skip 16384
stack_top:

.section .text





  gdt:
 
  gdt_null:
.long 0
.long 0 
  gdt_code:
  .word 0x0FFFF
  .word 0
 
  .byte 0
  .byte 0x9A
  .byte 0xCF
  .byte 0

  gdt_data:
  .word 0x0FFFF
  .word 0
 
  .byte 0
  .byte 0x92
  .byte 0xCF
  .byte 0
 
  gdt_end:
 
  gdt_desc:
   .word gdt_end - gdt - 1
   .long gdt
 


.global _start
_start:
    cli 
    lgdt gdt_desc

reloadSegments:

   jmp $0x08,$reload_CS 
reload_CS:
   MOV   $0x10, %ax
   MOV   %AX, %DS
   MOV   %AX, %ES
   MOV   %AX, %FS
   MOV   %AX, %GS
   MOV   %AX, %SS
   #RET
    #sti 
    call kernel_main
1:
    hlt
    jmp 1b
