CC=clang
CFLAGS=-std=c99 -Wall -Wextra -Wconversion -Wsign-conversion -Werror -pedantic
LDLIBS=-lraylib

DEBUG_FLAGS=-ggdb
RELEASE_FLAGS=-O3 -DNDEBUG

HEADERS=src/config.h src/main.h src/util.h

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
