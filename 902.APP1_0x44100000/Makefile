# Tool information
TOOL_DIR		= C:\CodeSourcery\Sourcery G++ Lite
HOME_DIR		= C:\OpenTFTPServer\home
VERSION			= 4.8.1
TARGET			= arm-none-eabi

# Project name & Link script
OUT_FILE_NAME 	= app1_0x44100000
LDS_FILE_NAME	= dram_0x44100000.lds

# Compiler & Linker Option
CFLAGS			= -mcpu=cortex-a9 -c -g -O3 -Wall -mfloat-abi=softfp -mfpu=neon-vfpv4 -fno-builtin -funsigned-char
CFLAGS		    += -ftree-vectorize -ftree-vectorizer-verbose=0 -fno-strict-aliasing -fno-common -pipe -mapcs
LDFLAGS			= --cref -Bstatic -nostdlib -p -EL

# Output File definition
OUT_BIN_FILE	= $(OUT_FILE_NAME).bin
OUT_ELF_FILE	= $(OUT_FILE_NAME).elf
OUT_MAP_FILE	= $(OUT_FILE_NAME).map
OUT_DUMP_FILE	= $(OUT_FILE_NAME).dmp

# Tool setting
AS				= "$(TOOL_DIR)/bin/$(TARGET)-as"
CC				= "$(TOOL_DIR)/bin/$(TARGET)-gcc"
LD 				= "$(TOOL_DIR)/bin/$(TARGET)-ld"
OBJCOPY			= "$(TOOL_DIR)/bin/$(TARGET)-objcopy"
OBJDUMP			= "$(TOOL_DIR)/bin/$(TARGET)-objdump"

# Source & Header File 
CSRC 			= $(wildcard *.c)
ASRC 			= $(wildcard *.s)
HEADER 			= $(wildcard *.h)
OBJS    		= $(CSRC:.c=.o) $(ASRC:.s=.o)
DUMP 			= $(wildcard *.txt)

# Library and include folder
C_DIR			= $(TOOL_DIR)/$(TARGET)
GCC_DIR			= $(TOOL_DIR)/lib/gcc/$(TARGET)/$(VERSION)
LIB_OPTION		= -L "$(C_DIR)/lib" -L "$(GCC_DIR)" -lc -lgcc
INCLUDE			= -nostdinc  -I. -I "$(C_DIR)/include" -I "$(GCC_DIR)/include" 

all : $(OUT_BIN_FILE)

$(OUT_BIN_FILE): $(OUT_ELF_FILE)
	$(OBJCOPY) $(OUT_ELF_FILE) $(OUT_BIN_FILE) -O binary
#	$(OBJDUMP) -x -D .\$(OUT_ELF_FILE) > .\__dump.txt
#	$(OBJDUMP) -x -D -S .\$(OUT_ELF_FILE) > .\__dump_all.txt
	copy /y $(OUT_BIN_FILE) "$(HOME_DIR)"

$(OUT_ELF_FILE): $(OBJS)
	$(LD) $(OBJS) -o $(OUT_ELF_FILE) $(LDFLAGS) -Map $(OUT_MAP_FILE) $(LIB_OPTION) -T $(LDS_FILE_NAME)

$(OBJS): $(ASRC) $(CSRC) $(HEADER)
	$(CC) $(ASRC) $(CSRC) $(INCLUDE) $(CFLAGS) 

clean :
	rm -f $(OUT_BIN_FILE)
	rm -f $(OUT_ELF_FILE)
	rm -f $(OUT_MAP_FILE)
	rm -f $(OBJS)
	rm -f $(DUMP)
	