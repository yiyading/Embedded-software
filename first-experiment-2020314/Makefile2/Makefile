program: programA.o programB.o
	gcc programA.o programB.o -o program
programA.o: programA.c programA.h
	gcc -g -O2 -c programA.c -o programA.o
programB.o: programB.c programB.h
	gcc -g -O2 -c programB.c -o programB.o
