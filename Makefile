CC = gcc
CFLAGS ?= -O2 -Wall -std=c11 -masm=intel -g
LDFLAGS ?= 

ODIR = ./bin

src = $(wildcard src/*.c src/*/*.c)
obj = $(src:.c=.o)

.PHONY: clean build

build: $(src)
	$(CC) $(src) $(CFLAGS) -o $(ODIR)/forth