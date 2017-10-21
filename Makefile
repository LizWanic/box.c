
all: box

box: box.c
	gcc -Wall box.c -o box

clean:
	rm -f box

dist:
	tar -cvzf dist2.tgz Makefile box.c
