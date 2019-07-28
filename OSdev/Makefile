GCCPARAMS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra
NASMPARAMS = -felf32
GCC = i686-elf-gcc
AS = i686-elf-as
GCCLINKING = -ffreestanding -O2 -nostdlib

ARCH_OBJS = $(patsubst %.s,%.o,$(wildcard arch/x86/*.c))
DRIVER_OBJS = $(patsubst %.c,%.o,$(wildcard driver/*.c))
KERNEL_OBJS = $(patsubst %.c,%.o,$(wildcard kernel/*.c))
ASM_OBJS = $(patsubst %.s,%.o,$(wildcard arch/x86/*.s))
ASM_OBJS += $(patsubst %.c,%.o,$(wildcard kernel/*.s))
LIBC_OBJS = $(patsubst %.c,%.o,$(wildcard libc/*.c))
INCLUDE_ASMOBJS = $(patsubst %.s,%.o,$(wildcard include/*.s))
CPU_OBJS = $(patsubst %.c,%.o,$(wildcard kernel/cpu/*.c))

KERNEL_OBJS += $(LIBC_OBJS)
KERNEL_OBJS += $(INCLUDE_ASMOBJS)
KERNEL_OBJS += $(ARCH_OBJS)
KERNEL_OBJS += $(ASM_OBJS)
KERNEL_OBJS += $(DRIVER_OBJS)
KERNEL_OBJS += $(CPU_OBJS)


libc/%.o : libc/%.c
	$(GCC) -c $< -o $@ $(GCCPARAMS)

kernel/%.o : kernel/%.c
	$(GCC) -c $< -o $@ $(GCCPARAMS)

arch/x86/%.o : arch/x86/%.s
	$(AS) $< -o $@

driver/%.o : driver/%.c  
	$(GCC) -c $< -o $@ $(GCCPARAMS)

kernel/cpu/%.o : kernel/cpu/%.c
	$(GCC) -c $< -o $@ $(GCCPARAMS)

myos.bin: arch/x86/linker.ld  $(KERNEL_OBJS)
	$(GCC) -T $< -o $@ $(GCCLINKING) $(KERNEL_OBJS)  -lgcc

myos.iso: myos.bin
	mkdir -p iso/boot/grub
	cp myos.bin iso/boot/myos.bin
	echo 'set timeout=0'> iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo ''
	echo 'menuentry "My Operating System" {' >> iso/boot/grub/grub.cfg
	echo '	multiboot /boot/myos.bin' >> iso/boot/grub/grub.cfg
	echo '  boot' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	grub-mkrescue -o myos.iso iso 
	rm -rf iso
run:
	export DISPLAY=:0
	qemu-system-i386 -cdrom myos.iso	

clean:
	rm libc/*\.o
	rm kernel/*\.o 
	rm arch/x86/*\.o
	rm driver/*\.o
	rm *\.bin
	rm kernel/cpu/*\.o
	rm include/*\.o
	