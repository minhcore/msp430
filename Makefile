SHELL := "C:/Program\ Files/Git\usr/bin/sh.exe"

# Directories
MSPGCC_ROOT_DIR = C:\tools
MSPGCC_BIN_DIR = $(MSPGCC_ROOT_DIR)/bin
MSPGCC_INCLUDE_DIR = $(MSPGCC_ROOT_DIR)/include
INCLUDE_DIRS = $(MSPGCC_INCLUDE_DIR)
LIB_DIRS = $(MSPGCC_INCLUDE_DIR)
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
BIN_DIR = $(BUILD_DIR)/bin
GDB = msp430-elf-gdb
GIT_USR_BIN := C:/Program Files/Git/usr/bin
GDB_AGENT_DIR = $(MSPGCC_BIN_DIR)
CPP_DIR := C:/Program Files/Cppcheck


# Toolchain
CC := $(MSPGCC_BIN_DIR)\msp430-elf-gcc
RM := $(GIT_USR_BIN)/rm.exe -rf
MKDIR := $(GIT_USR_BIN)/mkdir.exe -p
SIZE := $(MSPGCC_BIN_DIR)/msp430-elf-size
SLEEP := $(GIT_USR_BIN)/sleep.exe
CPPCHECK = $(CPP_DIR)/cppcheck.exe

# Tool Debug
GDB_AGENT := $(GDB_AGENT_DIR)/gdb_agent_console.exe
GDB_DAT_FILE := $(GDB_AGENT_DIR)/msp430.dat


# Files
TARGET = $(BIN_DIR)/blink

SOURCE = \
	src/main.c \
	external/printf/printf.c
		

OBJECT_NAMES = $(SOURCE:.c=.o)
OBJECTS = $(patsubst %, $(OBJ_DIR)/%, $(OBJECT_NAMES))


# Flags
MCU = msp430g2553
WFLAGS = -Wall -Wextra -Werror -Wshadow
CFLAGS = -mmcu=$(MCU) $(WFLAGS) $(addprefix -I,$(INCLUDE_DIRS)) -Og -g
LDFLAGS = -mmcu=$(MCU) $(addprefix -L,$(LIB_DIRS)) -Wl,-Map,$(TARGET).map

# Build
## Linking
$(TARGET): $(OBJECTS)
	$(MKDIR) $(dir $@)
	$(CC) $(LDFLAGS) $^ -o $@
	@echo "---Memory Usage---"
	$(SIZE) $@
	

## Compiling
$(OBJ_DIR)/%.o: %.c
	$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $^

# Phonies
.PHONY: all clean flash 

all: $(TARGET)

clean:
	$(RM) $(BUILD_DIR)

flash: 
	@echo "Checking Agent and Flashing..."
	start "MSP430 GDB Agent" "$(GDB_AGENT)" "$(GDB_DAT_FILE)"

	$(SLEEP) 2

	$(GDB) -batch \
	-ex "target remote :55000" \
	-ex "load" \
	-ex "continue" \
	-ex "quit" $(TARGET)
	@echo "Flash Successful!"

cppcheck:
	@$(CPPCHECK) --quiet --enable=all --error-exitcode=1 \
	--inline-suppr \
	-I $(INCLUDE_DIRS) \
	$(SOURCE) \
	-i external/printf


