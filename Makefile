
TOOLS_PATH ?= C:/tools
TOOLS_DIR = $(TOOLS_PATH)
MSPGCC_BIN_DIR = $(TOOLS_DIR)/bin
MSPGCC_INCLUDE_DIR = $(TOOLS_DIR)/include
SUPPORT_FILES_PATH ?= /opt/msp430-gcc-support/include

BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
BIN_DIR = $(BUILD_DIR)/bin


CC       = $(MSPGCC_BIN_DIR)/msp430-elf-gcc
SIZE     = $(MSPGCC_BIN_DIR)/msp430-elf-size
GDB      = $(MSPGCC_BIN_DIR)/msp430-elf-gdb
CPPCHECK = cppcheck
MKDIR    = mkdir -p
RM       = rm -rf


ifeq ($(OS),Windows_NT)
    
endif


MCU         = msp430g2553
MCU_DEFINE  = __MSP430G2553__
TARGET      = $(BIN_DIR)/blink.elf

INCLUDE_DIRS = $(MSPGCC_INCLUDE_DIR) ./src ./external ./
SOURCE = src/main.c external/printf/printf.c

OBJECT_NAMES = $(SOURCE:.c=.o)
OBJECTS      = $(patsubst %, $(OBJ_DIR)/%, $(OBJECT_NAMES))


WFLAGS  = -Wall -Wextra -Werror -Wshadow
CFLAGS = -mmcu=msp430g2553 $(WFLAGS) -I$(SUPPORT_FILES_PATH) $(addprefix -I,$(INCLUDE_DIRS)) -Og -g
LDFLAGS = -mmcu=$(MCU) -L$(MSPGCC_INCLUDE_DIR) -Wl,-Map,$(TARGET).map


.PHONY: all clean flash cppcheck 

all: $(TARGET)

# Linking
$(TARGET): $(OBJECTS)
	@$(MKDIR) $(dir $@)
	$(CC) $(LDFLAGS) $^ -o $@
	@echo "--- Memory Usage ---"
	$(SIZE) $@

# Compiling
$(OBJ_DIR)/%.o: %.c
	@$(MKDIR) $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) $(BUILD_DIR)


cppcheck:
	@echo "--- Running Static Analysis ---"
	@$(CPPCHECK) --quiet --enable=all --error-exitcode=1 \
		--inline-suppr \
		--suppress=missingIncludeSystem \
		--suppress=unmatchedSuppression \
		--suppress=unusedFunction \
		$(addprefix -I,$(INCLUDE_DIRS)) \
		-D $(MCU_DEFINE) \
		$(SOURCE) \
		-i external/printf


TO_WIN_PATH = $(subst /,\,$(1))
GDB_AGENT   = $(MSPGCC_BIN_DIR)/gdb_agent_console.exe
GDB_DAT_FILE = $(MSPGCC_BIN_DIR)/msp430.dat

flash: $(TARGET)
ifeq ($(OS),Windows_NT)
	@echo "--- Preparing to launch Agent (Windows) ---"
	@echo 'start "MSP430 Agent" "$(call TO_WIN_PATH,$(GDB_AGENT))" "$(call TO_WIN_PATH,$(GDB_DAT_FILE))"' > run_agent.bat
	@cmd.exe /c run_agent.bat
	@rm run_agent.bat
	@sleep 2
	$(GDB) -batch -ex "target remote :55000" -ex "load" -ex "continue" -ex "quit" $(TARGET)
	@echo "Flash Successful!"
else
	@echo "Target 'flash' requires physical hardware and Windows drivers."
endif