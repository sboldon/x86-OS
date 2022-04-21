# TODO: Separate release builds and debug builds into their own build pipeline and build directory.
CC = i686-elf-gcc
AS = nasm
GDB = i386-elf-gdb
QEMU = qemu-system-i386

MKDIR ?= @mkdir -p

IMG ?= kernel.img
BUILD_DIR ?= _build
TARGET := $(BUILD_DIR)/$(IMG)

CFLAGS = -std=c11 -Wall -ffreestanding -g
ASFLAGS = -f elf -g
LDFLAGS = -Ttext 0x1000 -nostdlib -lgcc
QEMUFLAGS := -drive format=raw,media=disk,file=$(TARGET)

BOOT_DIR ?= boot
KERNEL_DIR ?= kernel
SRC_DIRS := $(KERNEL_DIR) cpu drivers lib

BOOT_SOURCES := $(wildcard $(BOOT_DIR)/*.asm)

# Ensure kernel entry code is built first and will be first in the generated binary file.
SOURCES := $(shell find $(SRC_DIRS) -name '*.c' -or -name '*.asm')
SOURCES := $(filter-out start.asm, $(SOURCES))
SOURCES := $(KERNEL_DIR)/start.asm $(SOURCES)

# Turn ./<dir>/<file>.<ext> into ./<BUILD_DIR>/<dir>/<file>.<ext>.o
OBJS := $(SOURCES:%=$(BUILD_DIR)/%.o)

# Automatically generate dependency files from source files using include directories.
DEPS := $(OBJS:.o=.d)
INC_DIRS := $(shell find $(SRC_DIRS) -type d -name "*include")
INC_FLAGS := $(addprefix -I,$(INC_DIRS))
CPPFLAGS ?= $(INC_FLAGS) -MMD -MP

####################################################################################################

.PHONY: all run run_debug clean

all: $(TARGET)

run: $(TARGET)
	$(QEMU)	$(QEMUFLAGS)

# QEMU option `-s` accepts a GDB connection on localhost:1234 and option `-S` freezes CPU on start
# up.
run_debug: $(TARGET) $(BUILD_DIR)/kernel.elf
	$(QEMU)	$(QEMUFLAGS) -s -S &
	$(GDB) -ex "target remote localhost:1234" -ex "symbol-file $(BUILD_DIR)/kernel.elf"

# Create bootable OS image.
$(TARGET): $(BUILD_DIR)/$(BOOT_DIR)/boot.bin $(BUILD_DIR)/kernel.bin
	@cat $^ > $@

$(BUILD_DIR)/kernel.bin: $(OBJS)
	$(CC) $^ -o $@ -Wl,--oformat=binary $(LDFLAGS)

# Symbol info for debugging with GDB.
$(BUILD_DIR)/kernel.elf: $(OBJS)
	$(CC) $^ -o $@ $(LDFLAGS)

# Kernel C sources.
$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR) $(dir $@)
	$(CC) -c $< -o $@ $(CPPFLAGS) $(CFLAGS)

# Kernel assembly sources.
$(BUILD_DIR)/%.asm.o: %.asm
	$(MKDIR) $(dir $@)
	$(AS) $< -o $@ $(ASFLAGS)

# Boot assembly sources.
$(BUILD_DIR)/%.bin: %.asm $(BOOT_SOURCES)
	$(MKDIR) $(dir $@)
	$(AS) $< -o $@ -f bin -I$(BOOT_DIR)

clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)
