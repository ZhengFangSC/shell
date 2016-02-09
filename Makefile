CC=gcc
CFLAGS=-Wall -g
SRCS=parser.c shell.c
OBJS=$(SRCS:.c=.o)
MAIN=shell


all: $(MAIN)

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm $(MAIN) *.o

run:
	./$(MAIN)
