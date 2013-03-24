CFLAGS=`pkg-config --cflags gtk+-3.0` -g
LDFLAGS=`pkg-config --libs gtk+-3.0`
OBJS=$(patsubst %.c,%.o,$(wildcard *.c))
ALL=PocketSphinxTrainer
MODELDIR=./lib/model
TRAINDIR=./lib/train

all: $(ALL)

PocketSphinxTrainer: $(OBJS)
	gcc -o PocketSphinxTrainer $(OBJS) $(LDFLAGS)

conf.o: conf.c
	cc $(CFLAGS)   -c -o conf.o conf.c -DMODELDIR=\"$(MODELDIR)\" -DTRAINDIR=\"$(TRAINDIR)\"

clean:
	rm -f PocketSphinxTrainer *.o *~
