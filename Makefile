ARCH	= x86_64

TARGET	= apple_set_os.efi
FORMAT 	= efi-app-$(ARCH)

GNU_EFI ?= /usr

INC     = $(GNU_EFI)/include/efi
CFLAGS	= -I$(INC) -I$(INC)/$(ARCH) \
		 -DGNU_EFI_USE_MS_ABI -fPIC -fshort-wchar -ffreestanding \
		 -fno-stack-protector -maccumulate-outgoing-args \
		 -Wall -D$(ARCH) -Werror -m64 -mno-red-zone

LDFLAGS	= -T $(GNU_EFI)/lib/elf_$(ARCH)_efi.lds -Bsymbolic -shared -nostdlib -znocombreloc \
		  $(GNU_EFI)/lib/crt0-efi-$(ARCH).o

%.efi: %.so
	objcopy -j .text -j .sdata -j .data -j .dynamic -j .dynsym -j .rel \
		-j .rela -j .reloc -S --target=$(FORMAT) $^ $@

%.so: %.o
	$(LD) $(LDFLAGS) -o $@ $^ $(shell $(CC) $(CFLAGS) -print-libgcc-file-name) \
	$(GNU_EFI)/lib/libgnuefi.a

all: $(TARGET)

clean:
	rm -f $(TARGET) *.so apple_set_os.o
