main: contas.c i-banco.c LinkedList.c commandlinereader.c comandos.c wrappers.c
	gcc -pthread -Wall -pedantic -g -o main contas.c i-banco.c LinkedList.c commandlinereader.c comandos.c wrappers.c
clean:
	rm *.o
