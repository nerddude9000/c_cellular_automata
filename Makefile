CC=gcc
CFLAGS=-lraylib -std=c99 -pedantic-errors -Wall -Wextra -Wconversion -Wsign-conversion -Werror
DEBUG_FLAGS=-ggdb
RELEASE_FLAGS=-O2 -DNDEBUG

all: debug release

run: debug
	./build/app_debug

debug: src/main.c build
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) -o build/app_debug src/main.c

release: src/main.c build
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) -o build/app_release src/main.c

build:
	mkdir -pv build