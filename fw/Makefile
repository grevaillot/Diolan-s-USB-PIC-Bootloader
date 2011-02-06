VID		?= 0x0000
PID		?= 0x0000
XTEA_KEY	?= "ABCDEFGHIJKLMNOP"

TOOLCHAIN	?= "gputils"
PROCESSOR	?= 18f4455
PROJECT		= bootloader

SRCS		= boot.asm boot_asm.asm usb.asm usb_desc.asm vectors.asm xtea.asm
INCS		= boot_if.inc boot.inc io_cfg.inc usb_defs.inc usb_desc.inc usb.inc P18F4455.INC
OBJS		= $(SRCS:.asm=.o)
LKR		= $(PROJECT).lkr

COF		= $(PROJECT).cof
HEX		= $(PROJECT).hex
MAP		= $(PROJECT).map

OBJ_DIR		= _output

ifneq ($(VID), "")
FLAGS += BOOTLOADER_VID=$(VID)
endif

ifneq ($(PID), "")
FLAGS += BOOTLOADER_PID=$(PID)
endif

ifneq ($(XTEA_KEY), "")
# XXX
# FLAGS += XTEA_KEY=$(XTEA_KEY)
endif

ifeq ($(TOOLCHAIN),"gputils")
	ASM		= gpasm
	LINK		= gplink
	MP2HEX		= mp2hex
	ASM_CMD		= $(ASM) -y -c -p $(PROCESSOR) $< -o $@ $(ASMFLAGS)
	LINK_CMD	= $(LINK) -m -o $@ -s $(LKR) $(OBJS)

	ASMFLAGS=$(addprefix -D ,$(FLAGS))
else
	ASM		= mpasmwin.exe
	LINK		= mplink.exe
	MP2HEX		= mp2hex.exe
	ASM_CMD		= $(ASM) /q /p$(PROCESSOR) $< /e /l /y /o$@ $(ASMFLAGS)
	LINK_CMD	= $(LINK) $(LKR) $(OBJS) /m$(MAP) /o$@
	COF2HEX_CMD	= $(MP2HEX) $(COF)

	ASMFLAGS=$(addprefix /d,$(FLAGS))
endif

all: $(HEX)

$(HEX): $(COF)
	$(COF2HEX_CMD)

$(COF): $(OBJS)
	$(LINK_CMD)

%.o: %.asm $(INCS)
	$(ASM_CMD)

clean:
	rm -f *.o *.ERR *.LST *.lst *.map *.cod *.cof *.hex
	rm -rf $(OBJ_DIR)

print:
	@echo "SRCS = $(SRCS)"
	@echo "INCS = $(INCS)"
	@echo "OBJS = $(OBJS)"
	@echo "FLAGS = $(FLAGS)"

.PHONY: all clean print
