all:
	gcc -std=c99 -o p2 mymalloc.h mymalloc.c memgrind.c

clean:
	rm -f p2
