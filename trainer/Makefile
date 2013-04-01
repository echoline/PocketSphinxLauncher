ALL=PocketSphinxTrainer
include ../defs.mk
CFLAGS=`pkg-config --cflags gtk+-3.0` -g -DMODELDIR=\"$(MODELDIR)\" -DHMMDIR=\"$(HMMDIR)\" -DLMDUMP=\"$(LMDUMP)\" -DLMDICT=\"$(LMDICT)\"
LDFLAGS=`pkg-config --libs gtk+-3.0`
OBJS=$(patsubst %.c,%.o,$(wildcard *.c))

all: $(ALL)

PocketSphinxTrainer: arctic20.h $(OBJS)
	gcc -o PocketSphinxTrainer $(OBJS) $(LDFLAGS)

arctic20.h: tools/makedoth
	tools/makedoth arctic20/* arctic20.h

clean:
	rm -f $(ALL) *.o *~
