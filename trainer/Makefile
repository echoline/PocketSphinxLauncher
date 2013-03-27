ALL=arctic20.h PocketSphinxTrainer
MODELDIR=/usr/local/share/pocketsphinx/model
TRAINDIR=train
HMMDIR=hmm/en_US/hub4wsj_sc_8k/
LMDUMP=lm/en_US/hub4.5000.DMP
LMDICT=lm/en_US/hub4.5000.dic
CFLAGS=`pkg-config --cflags gtk+-3.0` -g -DMODELDIR=\"$(MODELDIR)\" -DHMMDIR=\"$(HMMDIR)\" -DLMDUMP=\"$(LMDUMP)\" -DLMDICT=\"$(LMDICT)\" -DTRAINDIR=\"$(TRAINDIR)\"
LDFLAGS=`pkg-config --libs gtk+-3.0`
OBJS=$(patsubst %.c,%.o,$(wildcard *.c))

all: $(ALL)

arctic20.h: tools/makedoth
	cd arctic20 && for i in *; do ../tools/makedoth $$i ../arctic20.h; done;

PocketSphinxTrainer: $(OBJS)
	gcc -o PocketSphinxTrainer $(OBJS) $(LDFLAGS)

clean:
	rm -f PocketSphinxTrainer *.o *~ tools/makedoth arctic20.h
