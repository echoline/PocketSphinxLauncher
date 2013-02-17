CFLAGS=`pkg-config --cflags gtk+-3.0` -g
LDFLAGS=`pkg-config --libs gtk+-3.0`
OBJS=$(patsubst %.c,%.o,$(wildcard *.c))
ALL=PocketSphinxTrainer

all: $(ALL)

PocketSphinxTrainer: $(OBJS)
	gcc -o PocketSphinxTrainer $(OBJS) $(LDFLAGS)

clean:
	rm -f PocketSphinxTrainer *.o *~
