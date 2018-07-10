CROSS_COMPILE ?= arm-none-eabi-
CC = $(CROSS_COMPILE)gcc
OBJCOPY = $(CROSS_COMPILE)objcopy


ifeq ($(DEBUG_ME), 1)
CFLAGS += -DDEBUG_ME
endif

CFLAGS += -mthumb -mcpu=cortex-m0plus \
	-DSAMD -DSAMD21E18 -DBLACKMAGIC -I../libopencm3/include \
	-Iplatforms/samd -msoft-float -ffunction-sections -fdata-sections -MD

LINKER_SCRIPT="platforms/samd/samd.ld"
LDFLAGS = -mthumb -mcpu=cortex-m0plus -msoft-float -nostartfiles -lc $(CPU_FLAGS) -nodefaultlibs -T$(LINKER_SCRIPT) -Wl,--gc-sections \
	-L../libopencm3/lib -lopencm3_samd -lnosys -lm -lgcc

ifeq ($(ENABLE_DEBUG), 1)
LDFLAGS += --specs=rdimon.specs
else
LDFLAGS += --specs=nosys.specs
endif

VPATH += platforms/samd

SRC +=	cdcacm.c	\
	timing.c        \
	traceswo.o      \
	usbuart.c       \

all:	blackmagic.bin 

host_clean:
	-$(Q)$(RM) -f blackmagic.bin blackmagic_dfu blackmagic_dfu.bin blackmagic_dfu.hex

