#Changelog
# This Mac SDK installed in volume as the wiki
# http://www.esp8266.com/wiki/doku.php?id=setup-osx-compiler-esp8266
# The Makefile is OSX. but will work mostly on linux
# Changed the variables to include the header file directory
# Added global var for the XTENSA tool root
#
# This make file still needs some work.
#
#
# Output directors to store intermediate compiled files
# relative to the project directory
BUILD_BASE	= build
FW_BASE		= firmware

USE_HEATSHRINK ?= yes
GZIP_COMPRESSION ?= yes

# Base directory for the compiler
XTENSA_TOOLS_ROOT ?= /Volumes/esp-open-sdk/xtensa-lx106-elf/bin

# base directory of the ESP8266 SDK package, absolute
SDK_BASE        ?= /Volumes/esp-open-sdk/sdk

#Esptool.py path and port
ESPTOOL		?= /Volumes/esp-open-sdk/esptool/esptool.py
ESPPORT		?= /dev/tty.wchusbserial14*

# name for the target project
TARGET		= thebutton

#Position and maximum length of espfs in flash memory
ESPFS_POS = 0x12000
ESPFS_SIZE = 0x2E000

# which modules (subdirectories) of the project to include in compiling
ifdef BUTTON
# name for the target project
TARGET	= thebutton
MODULES	=  driver user lib/mqtt/mqtt lib/esphttpd/espfs lib/esphttpd/httpd 
CFLAGS	= -g -O2 -Os -Wpointer-arith -Wundef -Werror -Wl,-EL -fno-inline-functions -nostdlib -mlongcalls -mtext-section-literals  -D__ets__ -DICACHE_FLASH
endif

ifdef MQTT
# name for the target project
TARGET		= mqttlib
MODULES	=  lib/mqtt/mqtt 
CFLAGS	= -g -O2 -Os -Wpointer-arith -Wundef -Werror -Wl,-EL -fno-inline-functions -nostdlib -mlongcalls -mtext-section-literals  -D__ets__ -DICACHE_FLASH
LDSKIP = true
endif

ifdef HTTPD

# name for the target project
TARGET		= httpdlib
MODULES	=  lib/esphttpd/espfs lib/esphttpd/httpd lib/esphttpd/user 
CFLAGS	= -Os -ggdb  -std=c99 -Werror -Wpointer-arith -Wundef -Wall -Wl,-EL -fno-inline-functions \
		-nostdlib -mlongcalls -mtext-section-literals  -D__ets__ -DICACHE_FLASH -DGZIP_COMPRESSION\
		-Wno-address -DESPFS_POS=$(ESPFS_POS) -DESPFS_SIZE=$(ESPFS_SIZE) -DESPFS_HEATSHRINK
LDSKIP = true
CFLAGS		+= -DGZIP_COMPRESSION

endif

EXTRA_INCDIR	= include $(SDK_BASE)/../include lib/heatshrink lib/esphttpd/include lib/esphttpd/httpd lib/esphttpd/espfs lib/esphttpd/user lib/mqtt/include lib/mqtt/mqtt

# libraries used in this project, mainly provided by the SDK
LIBS		= c gcc hal phy pp net80211 lwip wpa upgrade main ssl

# compiler flags using during compilation of source files
CCFLAGS		+= -ffunction-sections -fno-jump-tables



# linker flags used to generate the main object file
LDFLAGS		= -nostdlib -Wl,--no-check-sections -u call_user_start -Wl,-static -DICACHE_FLASH

# linker script used for the above linkier step
#LD_SCRIPT	= eagle.app.v6.ld
LD_SCRIPT	= eagle.app.v6.New.ld

# various paths from the SDK used in this project
SDK_LIBDIR	= lib
SDK_LDDIR	= ld
SDK_INCDIR	= include include/json

# we create two different files for uploading into the flash
# these are the names and options to generate them
FW_FILE_1	= 0x00000
#FW_FILE_1_ARGS	= -bo $@ -bs .text -bs .data -bs .rodata -bc -ec
FW_FILE_ARGS	= elf2image -o firmware/

# select which tools to use as compiler, librarian and linker
CC		:= $(XTENSA_TOOLS_ROOT)/xtensa-lx106-elf-gcc
AR		:= $(XTENSA_TOOLS_ROOT)/xtensa-lx106-elf-ar
LD		:= $(XTENSA_TOOLS_ROOT)/xtensa-lx106-elf-gcc

####
#### no user configurable options below here
####

FW_TOOL		?= $(ESPTOOL)
SRC_DIR		:= $(MODULES)
BUILD_DIR	:= $(addprefix $(BUILD_BASE)/,$(MODULES))
SDK_LIBDIR	:= $(addprefix $(SDK_BASE)/,$(SDK_LIBDIR))
SDK_INCDIR	:= $(addprefix -I$(SDK_BASE)/,$(SDK_INCDIR))


SRC		:= $(foreach sdir,$(SRC_DIR),$(wildcard $(sdir)/*.c))
OBJ		:= $(patsubst %.c,$(BUILD_BASE)/%.o,$(SRC))
LIBS		:= $(addprefix -l,$(LIBS))
APP_AR		:= $(addprefix $(BUILD_BASE)/,$(TARGET)_app.a)
TARGET_OUT	:= $(addprefix $(BUILD_BASE)/,$(TARGET).out)

#LD_SCRIPT	:= $(addprefix -T$(SDK_BASE)/$(SDK_LDDIR)/,$(LD_SCRIPT))
LD_SCRIPT      := $(addprefix -T,$(LD_SCRIPT))

INCDIR	:= $(addprefix -I,$(SRC_DIR))
EXTRA_INCDIR	:= $(addprefix -I,$(EXTRA_INCDIR))
MODULE_INCDIR	:= $(addsuffix /include,$(INCDIR))

FW_FILE	:= $(addprefix $(FW_BASE)/,$(FW_FILE_1).bin)
FW_FILE_2	:= $(addprefix $(FW_BASE)/,$(FW_FILE_2).bin)
BLANKER	:= $(addprefix $(SDK_BASE)/,bin/blank.bin)

V ?= $(VERBOSE)
ifeq ("$(V)","1")
Q :=
vecho := @true
else
Q := @
vecho := @echo
endif

vpath %.c $(SRC_DIR)

define compile-objects
$1/%.o: %.c
	$(vecho) "CC $$<"
	$(Q) $(CC) $(INCDIR) $(MODULE_INCDIR) $(EXTRA_INCDIR) $(SDK_INCDIR) $(CFLAGS) -c $$< -o $$@
endef

.PHONY: all checkdirs clean

ifdef C99
all: checkdirs $(TARGET_OUT)
else
all: checkdirs $(TARGET_OUT) $(FW_FILE)
endif

$(FW_FILE): $(TARGET_OUT)
ifndef LDSKIP
	$(vecho) "FW $@"
	$(Q) $(FW_TOOL) $(FW_FILE_ARGS) $(TARGET_OUT) 
endif

$(TARGET_OUT): $(APP_AR)
ifndef LDSKIP
	$(vecho) "LD $@"
	$(Q) $(LD) -L$(SDK_LIBDIR) $(LD_SCRIPT) $(LDFLAGS) -Wl,--start-group $(LIBS) $(APP_AR) -Wl,--end-group -o $@
endif
$(APP_AR): $(OBJ)
ifndef LDSKIP
	$(vecho) "AR $@"
	$(Q) $(AR) cru $@ $^
endif

checkdirs: $(BUILD_DIR) $(FW_BASE)

$(BUILD_DIR):
	$(Q) mkdir -p $@

firmware:
	$(Q) mkdir -p $@

flash: $(FW_FILE) webpages.espfs
	$(Q) $(PYTHON) $(ESPTOOL) -p $(ESPPORT) write_flash 0x00000 firmware/0x00000.bin 0x3c000 firmware/0x3c000.bin $(ESPFS_POS) webpages.espfs

webpages.espfs: html/ mkespfsimage
	$(Q) cd html; find . | ../mkespfsimage > ../webpages.espfs; cd ..

mkespfsimage: lib/esphttpd/espfs/mkespfsimage/
	$(Q) make -C lib/esphttpd/espfs/mkespfsimage USE_HEATSHRINK="$(USE_HEATSHRINK)" GZIP_COMPRESSION="$(GZIP_COMPRESSION)"
	$(Q) mv lib/esphttpd/espfs/mkespfsimage/mkespfsimage ./

hardresetflash:
	$(Q) $(PYTHON) $(ESPTOOL) -p $(ESPPORT) write_flash 0x7e000 $(BLANKER) 0x00000 $(BLANKER) 0x3c000 $(BLANKER) $(ESPFS_POS) $(BLANKER) 0x2c000 $(BLANKER)

resetflash:
	$(Q) $(PYTHON) $(ESPTOOL) -p $(ESPPORT) write_flash 0x00000 $(BLANKER) 0x3c000 $(BLANKER) 0x12000 $(BLANKER) 0x2c000 $(BLANKER)

blankflash:
	$(Q) $(PYTHON) $(ESPTOOL) --port $(ESPPORT) --baud $(ESPBAUD) write_flash 0x7E000 $(SDK_BASE)/bin/blank.bin
	
htmlflash: webpages.espfs
	if [ $$(stat -f '%z' webpages.espfs) -gt $$(( 0x2E000 )) ]; then echo "webpages.espfs too big!"; false; fi
	$(PYTHON) $(ESPTOOL) -p $(ESPPORT) write_flash 0x12000 webpages.espfs

test: flash
	$(vecho) screen $(ESPPORT) 115200

clean:
	$(Q) rm -f $(APP_AR)
	$(Q) rm -f $(TARGET_OUT)
	$(Q) rm -rf $(BUILD_DIR)
	$(Q) rm -rf $(BUILD_BASE)
	$(Q) rm -f $(FW_FILE_1)
	$(Q) rm -f $(FW_FILE_2)
	$(Q) rm -rf $(FW_BASE)
	$(Q) rm -f mkespfsimage
	$(Q) rm -f webpages.espfs
	$(Q) rm -f lib/esphttpd/espfs/mkespfsimage/mkespfsimage
	$(Q) rm -f lib/esphttpd/espfs/mkespfsimage/*.o

$(foreach bdir,$(BUILD_DIR),$(eval $(call compile-objects,$(bdir))))
