all: CowShell

CowShell: main.o
	gcc tmp/main.o -o bin/CowShell

main.o:
	gcc -c src/main.c -o tmp/main.o

clean:
	rm -rf tmp/*.o bin/CowShell
