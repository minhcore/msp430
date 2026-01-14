ifeq ($(OS),Windows_NT)
	TOOLS_PATH ?= C:/tools
	SUPPORT_FILES_PATH ?= C:/tools/include
else
	TOOLS_PATH ?= /opt/msp430-gcc-9.3.1.11_linux64
	SUPPORT_FILES_PATH ?= /opt/msp430-gcc-support/msp430-gcc-support-files/include
endif


TOOLS_DIR = $(TOOLS_PATH)
MSPGCC_BIN_DIR = $(TOOLS_DIR)/bin
MSPGCC_INCLUDE_DIR = $(TOOLS_DIR)/include
LIB_DIRS = $(MSPGCC_INCLUDE_DIR) $(SUPPORT_FILES_PATH)

BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
BIN_DIR = $(BUILD_DIR)/bin


CC       = $(MSPGCC_BIN_DIR)/msp430-elf-gcc
SIZE     = $(MSPGCC_BIN_DIR)/msp430-elf-size
GDB      = $(MSPGCC_BIN_DIR)/msp430-elf-gdb
CPPCHECK = cppcheck
FORMAT	 = clang-format
RM = rm -rf
MKDIR = mkdir -p

MCU         = msp430g2553
MCU_DEFINE  = __MSP430G2553__
TARGET      = $(BIN_DIR)/blink.elf


INCLUDE_DIRS = $(MSPGCC_INCLUDE_DIR) ./src ./external ./
SOURCE = \
	src/main.c \


OBJECT_NAMES = $(SOURCE:.c=.o)
OBJECTS      = $(patsubst %, $(OBJ_DIR)/%, $(OBJECT_NAMES))


WFLAGS  = -Wall -Wextra -Werror -Wshadow
CFLAGS = -mmcu=msp430g2553 $(WFLAGS) -I$(SUPPORT_FILES_PATH) $(addprefix -I,$(INCLUDE_DIRS)) -Og -g
LDFLAGS = -mmcu=$(MCU) $(addprefix -L,$(LIB_DIRS)) -T $(SUPPORT_FILES_PATH)/$(MCU).ld -Wl,-Map,$(TARGET).map

CPPCHECK_INC = ./src ./
IGNORE_FILES = external/printf/
SOURCES_TO_CHECK = $(filter-out $(IGNORE_FILES)%,$(SOURCE))
CPPCHECK_BUILD_DIR = $(BUILD_DIR)/cppcheck_info

.PHONY: all clean flash cppcheck format

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
	@mkdir -p $(CPPCHECK_BUILD_DIR)
	@echo "--- Running Static Analysis ---"
	@$(CPPCHECK) --quiet --enable=warning,style,performance,portability --error-exitcode=1 \
		-j 4 \
		--max-configs=1 \
		--cppcheck-build-dir=$(CPPCHECK_BUILD_DIR) \
		--inline-suppr \
		--suppress=missingIncludeSystem \
		--suppress=unmatchedSuppression \
		--suppress=unusedFunction \
		$(addprefix -I,$(CPPCHECK_INC)) \
		-D $(MCU_DEFINE) \
		$(SOURCES_TO_CHECK) \
		


TO_WIN_PATH = $(subst /,\,$(1))
GDB_AGENT   = $(MSPGCC_BIN_DIR)/gdb_agent_console.exe
GDB_DAT_FILE = $(MSPGCC_BIN_DIR)/msp430.dat

format:
	@$(FORMAT) -i $(SOURCE)


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