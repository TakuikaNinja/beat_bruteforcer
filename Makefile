CC=cosmocc/bin/cosmocc

BUILDTARGET = beat_bruteforcer

all: $(BUILDTARGET)

$(BUILDTARGET): $(wildcard *.c)
	$(CC) -o $(BUILDTARGET).com *.c

clean:
	rm *.com *.dbg *.elf

