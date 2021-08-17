# Create binary file of bootloader
# boot.asm print.asm load_disk.asm gdt.asm prot_mode.asm prot_print.asm
boot.bin: *.asm
	$(AS) $< -o $@ -f bin 

.PHONY: clean

clean:
	rm *.bin
