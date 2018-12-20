CROSS_COMPILE ?= arm-none-eabi-
CC = $(CROSS_COMPILE)gcc
OBJCOPY = $(CROSS_COMPILE)objcopy

ifeq ($(ENABLE_DEBUG), 1)
CFLAGS += -DDEBUG_ME
CFLAGS += -DENABLE_DEBUG
endif

ifeq ($(CUSTOM_SER), 1)
CFLAGS += -DCUSTOM_SER
endif

CFLAGS += -mthumb -mcpu=cortex-m0plus \
	-DSAMD -DSAMD21E17 -DBLACKMAGIC -I../libopencm3/include \
	-Iplatforms/samd -msoft-float -ffunction-sections -fdata-sections -MD

LINKER_SCRIPT="platforms/samd/samd.ld"
LDFLAGS = -mthumb -mcpu=cortex-m0plus -msoft-float -nostartfiles -lc \
	$(CPU_FLAGS) -T$(LINKER_SCRIPT) -Wl,--gc-sections \
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

all:	blackmagic_full.bin blackmagic.bin blackmagic_dfu.bin blackmagic_dfu.hex

blackmagic_dfu.bin : OBJCOPY_FLAGS := --pad-to 0x00002000 --gap-fill 0xFF -j .text -j .data
blackmagic_dfu.bin : LINKER_SCRIPT := "platforms/samd/samd_boot.ld"

blackmagic_dfu: usbdfu.o
	@echo "  LD      $@"
	$(Q)$(CC) $^ -o $@ $(LDFLAGS)

blackmagic_full.bin: blackmagic_dfu.bin blackmagic.bin
	@echo "  CAT     $@"
	$(Q)cat $^ > $@

host_clean:
	-$(Q)$(RM) -f blackmagic.bin blackmagic_full.bin blackmagic_dfu blackmagic_dfu.bin blackmagic_dfu.hex

