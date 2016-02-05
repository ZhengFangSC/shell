all: shell

shell:
	g++ -g shell.c

clean:
	rm a.out

run:
	./a.out
