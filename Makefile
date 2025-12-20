CC=gcc
CFLAGS=-std=c99 -pedantic-errors -Wall -Wextra -Wconversion -Wsign-conversion -Werror
LDLIBS=-lraylib
DEBUG_FLAGS=-ggdb
RELEASE_FLAGS=-O2 -DNDEBUG

HEADERS=

.PHONY: all 
all: build/app_debug build/app_release

build/app_debug: src/main.c $(HEADERS) | build
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(LDLIBS) -o $@ $<

build/app_release: src/main.c $(HEADERS) | build
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) $(LDLIBS) -o $@ $<


.PHONY: run debug release build clean
run: debug
	./build/app_debug

debug: build/app_debug

release: build/app_release

build:
	mkdir -pv build

clean:
	rm -r build
