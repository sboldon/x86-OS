CC = i686-elf-gcc
AS = nasm
CFLAGS = -std=c11 -Wall -ffreestanding
ASFLAGS = -f elf
LDFLAGS = -Wl,--oformat=binary -Ttext 0x1000 -nostdlib -lgcc

TARGET ?= kernel.img
BUILD_DIR ?= _build
BOOT_DIR ?= boot
KERNEL_DIR ?= kernel
SRC_DIRS ?= $(KERNEL_DIR) drivers

BOOT_SRCS := $(wildcard $(BOOT_DIR)/*.asm)

# Ensure kernel entry code is built first and will be first in the generated binary file.
SRCS := $(shell find $(SRC_DIRS) -name *.c -or -name *.asm -and -name ^start.asm)
SRCS := $(KERNEL_DIR)/start.asm $(SRCS)

# Turn ./<dir>/<file>.<ext> into ./<BUILD_DIR>/<dir>/<file>.o
OBJS := $(foreach file, $(SRCS), $(basename $(file)))
OBJS := $(OBJS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

# Automatically generate dependency files from source files.
CPPFLAGS ?= $(INC_FLAGS) -MMD -MP


.PHONY: all

all: $(BUILD_DIR)/$(TARGET)

# Create bootable OS image
$(BUILD_DIR)/$(TARGET): $(BUILD_DIR)/$(BOOT_DIR)/boot.bin $(BUILD_DIR)/kernel.bin
	cat $^ > $@

$(BUILD_DIR)/kernel.bin: $(OBJS)
	$(CC) $^ -o $@ $(LDFLAGS)

# C source
$(BUILD_DIR)/%.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) -c $< -o $@ $(CPPFLAGS) $(CFLAGS)

# Kernel assembly
$(BUILD_DIR)/%.o: %.asm
	$(MKDIR_P) $(dir $@)
	$(AS) $< -o $@ $(ASFLAGS)

# Boot assembly
$(BUILD_DIR)/%.bin: %.asm $(BOOT_SRCS)
	$(MKDIR_P) $(dir $@)
	$(AS) $< -o $@ -f bin -I$(BOOT_DIR)

.PHONY: clean
clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)

MKDIR_P ?= @mkdir -p

