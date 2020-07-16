CC=gcc

LFLAGS=-lfreeimage
CFLAGS=-ggdb -O3 -Wall

CFILES = $(wildcard *.c)

pixelsort: $(CFILES)
	$(CC) $(CFLAGS) $(LFLAGS) -o $@ $^

.PHONY: run clean

run: pixelsort
	./pixelsort

clean:
	rm pixelsort
