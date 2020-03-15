myproj: main.o barrel.o char2int.o
	gcc -Wall main.o barrel.o char2int.o -o myproj
main.o: main.c
	gcc -Wall -c main.c -o main.o
barrel.o: barrel.c barrel.h
	gcc -Wall -c barrel.c -o barrel.o
char2int.o: char2int.c char2int.h
	gcc -Wall -c char2int.c -o char2int.o
