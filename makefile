CC=gcc
CFLAGS=-I.
DEPS = contas.h LinkedList.h commandlinereader.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: contas.o i-banco.o LinkedList.o commandlinereader.o
	gcc -o main contas.o i-banco.o LinkedList.o	commandlinereader.o

