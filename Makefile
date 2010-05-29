all: main

main: input.o wizard.o
	gcc -lpthread main.c input.o wizard.o

input.o: input.c
	gcc -c input.c

wizard.o: wizard.c
	gcc -c wizard.c

