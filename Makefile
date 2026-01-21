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

ifneq ($(TEST),)
TARGET_NAME = $(TEST).elf
MAIN_FILE = src/test/test.c
$(shell touch src/test/test.c)
TEST_DEFINE = -DTEST=$(TEST)
else 
TARGET_NAME = $(BIN_DIR)/firmware.elf
MAIN_FILE = src/main.c
TEST_DEFINE =
endif

TARGET = $(BIN_DIR)/$(TARGET_NAME)

INCLUDE_DIRS = $(MSPGCC_INCLUDE_DIR) ./src ./external ./
SOURCE = \
	$(MAIN_FILE) \
	src/drivers/io.c \
	src/drivers/mcu_init.c \
	src/common/assert_handler.c \
	src/drivers/led.c \
	src/drivers/uart.c

H_SOURCE = \
	src/drivers/io.h \
	src/drivers/mcu_init.h \
	src/drivers/led.h \
	src/drivers/uart.h
	
	

OBJECT_NAMES = $(SOURCE:.c=.o)
OBJECTS      = $(patsubst %, $(OBJ_DIR)/%, $(OBJECT_NAMES))


WFLAGS  = -Wall -Wextra -Werror -Wshadow
CFLAGS = -mmcu=$(MCU) $(WFLAGS) -fshort-enums -I$(SUPPORT_FILES_PATH) $(addprefix -I,$(INCLUDE_DIRS)) $(TEST_DEFINE) -Og -g
LDFLAGS = -mmcu=$(MCU) $(addprefix -L,$(LIB_DIRS)) -T $(SUPPORT_FILES_PATH)/$(MCU).ld -Wl,-Map,$(TARGET).map

CPPCHECK_INC = ./src ./
IGNORE_FILES = external/printf/
SOURCES_TO_CHECK = $(filter-out $(IGNORE_FILES)%,$(SOURCE))
CPPCHECK_BUILD_DIR = $(BUILD_DIR)/cppcheck_info

.PHONY: all clean flash cppcheck format tests

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
	@$(FORMAT) -i $(SOURCE) $(H_SOURCE)

tests:
	@chmod +x tools/build_tests.sh
	@./tools/build_tests.sh

flash: $(TARGET)
ifeq ($(OS),Windows_NT)
	@echo "--- Preparing to launch Agent (Windows) ---"
	@echo 'start "MSP430_Agent" "$(call TO_WIN_PATH,$(GDB_AGENT))" "$(call TO_WIN_PATH,$(GDB_DAT_FILE))"' > run_agent.bat
	@cmd.exe /c run_agent.bat
	@rm run_agent.bat
	@sleep 1
	$(GDB) -batch \
		-ex "target remote :55000" \
		-ex "load" \
		-ex "continue" \
		-ex "quit" \
		$(TARGET)
endif