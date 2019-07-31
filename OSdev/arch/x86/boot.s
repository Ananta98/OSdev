.set FLAGS, (1 << 0) | (1 << 1)
.set MAGIC, 0x1BADB002 
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
.extern main
.extern _init
.global _start
.type _start, @function
_start:
		mov $stack_top,%esp
		call _init
		call main

		cli
1:		hlt
		jmp 1b
.end:
.size _start, . - _start