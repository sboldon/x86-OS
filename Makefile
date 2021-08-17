export CC=i686-elf-gcc
export AS=nasm

all: os-image

os-image: boot.bin kernel.bin
	cat ./boot/$^ > $@

boot.bin:
	$(MAKE) -C ./boot

kernel.bin: kernel.o start.o
	$(CC) $^ -o $@ -Wl,--oformat=binary -Ttext 0x1000 -nostdlib -lgcc

kernel.o: kernel.c
	$(CC) -c $< -o $@ -ffreestanding

start.o: start.asm 
	$(AS) $< -o $@ -f elf 


.PHONY: clean
clean:
	rm *.o *.bin


	
