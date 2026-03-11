CC     ?= gcc
CFLAGS  = -Wall -Wextra -O2
# Linux/macOS: link iconv. Windows: make test LDFLAGS=
LDFLAGS ?= -liconv

.PHONY: all clean test

all: utf8.o

utf8.o: utf8.c utf8.h
	$(CC) $(CFLAGS) -c utf8.c -o utf8.o

test: utf8.o test_utf8.c
	$(CC) $(CFLAGS) -o test_utf8 test_utf8.c utf8.o $(LDFLAGS)
	./test_utf8

clean:
	rm -f utf8.o test_utf8
