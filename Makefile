CFLAGS=`pkg-config --cflags gtk+-3.0` -g
LDFLAGS=`pkg-config --libs gtk+-3.0 sphinxbase pocketsphinx` -lpthread -g
OBJS=$(patsubst %.c,%.o,$(wildcard *.c))
ALL=PocketSphinxLauncher trainer
INSTALLPATH=/usr/local
MODELDIR=./lib/model

all: $(ALL)

PocketSphinxLauncher: $(OBJS)
	gcc -o PocketSphinxLauncher $(OBJS) $(LDFLAGS)

trainer: force
	$(MAKE) -C trainer

sphx.o: sphx.c
	cc `pkg-config --cflags sphinxbase pocketsphinx` -g -c -o sphx.o sphx.c

conf.o: conf.c
	cc $(CFLAGS) -o conf.o -c conf.c -DMODELDIR=\"$(MODELDIR)\"

clean:
	rm -f PocketSphinxLauncher *.o *~
	make -C trainer clean

install: $(ALL)
	cp PocketSphinxLauncher trainer/PocketSphinxTrainer trainer/PSTrainer.sh $(INSTALLPATH)/bin

uninstall: $(ALL)
	rm $(INSTALLPATH)/bin/PocketSphinxLauncher $(INSTALLPATH)/bin/PocketSphinxTrainer $(INSTALLPATH)/bin/PSTrainer.sh

force:
	true
