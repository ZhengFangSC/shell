# mp2
A shell for Unix systems

My current thought process is that I'm handling newlines in the wrong place.
BASH and ZSH are languages that have direct access to programs in the `PATH` and `path`
variables, meaning they handle parsing before deciding whether or not to add
a newline. They also allow neat syntax like `echo "Neat thing: $(cat hi)"`.

If I want to truly create a shell, I have to create this language.
