TARGET  = libjailbreak.dylib
OUTDIR ?= .

CC      = xcrun -sdk iphoneos gcc -arch arm64 -arch arm64e
LDID    = ldid

.PHONY: all clean

all: $(OUTDIR)/$(TARGET)

$(OUTDIR)/$(TARGET): libjelbrek.m jailbreak_daemonUser.c
	$(CC) -dynamiclib -I./ -o $@ $^
	$(LDID) -S $@

clean:
	rm -rf $(OUTDIR)/$(TARGET)
