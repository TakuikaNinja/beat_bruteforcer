CC=cosmocc/bin/cosmocc

BUILDTARGET = beat_bruteforcer
FLAGS = -mtiny -Oz

all: $(BUILDTARGET)

$(BUILDTARGET): $(wildcard *.c)
	$(CC) $(FLAGS) -o $(BUILDTARGET).com *.c

clean:
	rm *.com *.dbg *.elf

