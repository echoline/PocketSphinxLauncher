CFLAGS=`pkg-config --cflags gtk+-3.0` -g
LDFLAGS=`pkg-config --libs gtk+-3.0 sphinxbase pocketsphinx` -lpthread -g
OBJS=$(patsubst %.c,%.o,$(wildcard *.c))
ALL=PocketSphinxLauncher trainer
INSTALLPATH=/home/eli/Documents/PSLauncher

all: $(ALL)

PocketSphinxLauncher: $(OBJS)
	gcc -o PocketSphinxLauncher $(OBJS) $(LDFLAGS)

trainer: force
	$(MAKE) -C trainer

sphx.o: sphx.c
	cc `pkg-config --cflags sphinxbase pocketsphinx` -g -c -o sphx.o sphx.c

clean:
	rm -f PocketSphinxLauncher *.o *~
	make -C trainer clean

install: $(ALL)
	cp PocketSphinxLauncher trainer/PocketSphinxTrainer $(INSTALLPATH)/bin

uninstall: $(ALL)
	rm $(INSTALLPATH)/bin/PocketSphinxLauncher $(INSTALLPATH)/bin/PocketSphinxTrainer

force:
	true
