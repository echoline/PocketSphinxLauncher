CFLAGS=`pkg-config --cflags gtk+-3.0` -g
LDFLAGS=`pkg-config --libs gtk+-3.0` -g
OBJS=$(patsubst %.c,%.o,$(wildcard *.c))

PocketSphinxLauncher: $(OBJS)
	gcc -o PocketSphinxLauncher $(OBJS) $(LDFLAGS)

clean:
	rm -f PocketSphinxLauncher *.o
