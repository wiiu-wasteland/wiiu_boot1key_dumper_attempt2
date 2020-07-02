.SUFFIXES:

LD	:=	powerpc-linux-gnu-ld
CC	:=	powerpc-linux-gnu-gcc

SOURCES		:=	.
BUILD		:=	build
TARGET 		:= 	ppc-hax-kernel

LDFLAGS	:=	-n

CFLAGS	:=	-Wall -Wundef -Wstrict-prototypes -Wno-trigraphs \
		-fno-strict-aliasing -Os -msoft-float -mno-altivec -mno-vsx \
		-pipe -fomit-frame-pointer -fno-builtin -fPIC -nostdinc \
		-m32 -mbig-endian -isystem $(shell $(CC) -print-file-name=include) \
		-fno-stack-protector

ASFLAGS	:=	-D__ASSEMBLY__ $(CFLAGS) -traditional -nostdinc

CFLAGS	+=	-g -I.

CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.S)))
BINFILES	:=	$(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.*)))

export OFILES	:=	$(addsuffix .o,$(BINFILES)) \
					$(CFILES:.c=.o) $(SFILES:.S=.o)

all: $(TARGET).elf

$(TARGET).elf: link.ld $(OFILES)
	$(LD) -T $^ $(LDFLAGS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.S
	$(CC) $(ASFLAGS) -c $< -o $@

clean:
	rm -f $(OFILES) $(TARGET).elf
