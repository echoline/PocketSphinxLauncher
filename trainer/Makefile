ALL=PocketSphinxTrainer
include ../defs.mk
CFLAGS=`pkg-config --cflags gtk+-3.0` -g -DMODELDIR=\"$(MODELDIR)\" -DHMMDIR=\"$(HMMDIR)\" -DLMDUMP=\"$(LMDUMP)\" -DLMDICT=\"$(LMDICT)\"
LDFLAGS=`pkg-config --libs gtk+-3.0`
OBJS=$(patsubst %.c,%.o,$(wildcard *.c))

all: $(ALL)

PocketSphinxTrainer: $(OBJS)
	gcc -o PocketSphinxTrainer $(OBJS) $(LDFLAGS)

clean:
	rm -f PocketSphinxTrainer *.o *~ tools/makedoth arctic20.h
