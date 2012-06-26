CFLAGS=`pkg-config --cflags gtk+-3.0` -g
LDFLAGS=`pkg-config --libs gtk+-3.0 sphinxbase pocketsphinx` -lpthread -g
OBJS=$(patsubst %.c,%.o,$(wildcard *.c))

PocketSphinxLauncher: $(OBJS)
	gcc -o PocketSphinxLauncher $(OBJS) $(LDFLAGS)

sphx.o: sphx.c
	cc `pkg-config --cflags sphinxbase pocketsphinx` -g -c -o sphx.o sphx.c

clean:
	rm -f PocketSphinxLauncher *.o
