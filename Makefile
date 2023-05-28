CC=gcc
OBJCOPY=objcopy

BUILDTARGET = beat_bruteforcer$(TYPE).com

all: $(BUILDTARGET)

$(BUILDTARGET): $(wildcard *.c)
	$(CC) -g -Os -static -nostdlib -nostdinc -fno-pie -no-pie -mno-red-zone \
	-fno-omit-frame-pointer -pg -mnop-mcount -mno-tls-direct-seg-refs -gdwarf-4 \
	-o $(BUILDTARGET).dbg *.c -fuse-ld=bfd -Wl,-T,ape.lds -Wl,--gc-sections \
	-Wl,--build-id=none -include cosmopolitan.h crt.o ape-no-modify-self.o cosmopolitan.a
	$(OBJCOPY) -S -O binary $(BUILDTARGET).dbg $(BUILDTARGET)

clean:
	rm *.com *.dbg

