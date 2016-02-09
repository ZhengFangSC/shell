CC=gcc
CFLAGS=-Wall -g
SRCS=parser.c shell.c
OBJS=$(SRCS:.c=.o)
MAIN=shell
RM=rm


all: $(MAIN)

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(MAIN) *.o

run:
	./$(MAIN)
