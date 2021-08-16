# Create binary file
boot: boot.asm
	nasm -f bin $< -o $@.bin

.PHONY: clean

clean:
	rm *.bin
