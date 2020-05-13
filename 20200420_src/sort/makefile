CC=arm-linux-gnueabihf-gcc
myproj: main.o barrel.o char2int.o
	$(CC) -g -Wall main.o barrel.o char2int.o -o myproj
main.o: main.c
	$(CC) -g -Wall -c main.c -o main.o
barrel.o: barrel.c barrel.h
	$(CC) -g -Wall -c barrel.c -o barrel.o
char2int.o: char2int.c char2int.h
	$(CC) -g -Wall -c char2int.c -o char2int.o
clean:
	rm myproj main.o barrel.o char2int.o
