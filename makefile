CC=clang
CFLAGS=-std=c99 -Wall -Wextra -pedantic -fdiagnostics-color=always
CFLAGS+=-O0 -g3 -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls
LDFLAGS+=-fsanitize=address
LINKER_FLAGS=-lSDL2
SRC=source/
INC=source/include/
BIN=binary/

ALL_EXECUTABLES= emulator translator

all: $(ALL_EXECUTABLES) clean

emulator: emulator.o cpu.o display.o
	$(CC) $(LDFLAGS) $(LINKER_FLAGS) $^ -o $@

test_file: test_file.o cpu.o display.o
	$(CC) $(LDFLAGS) $(LINKER_FLAGS) $^ -o $@

emulator.o: $(SRC)emulator.c $(INC)cpu.h $(INC)display.h
	$(CC) $(CFLAGS) -c -o $@ $<

cpu.o: $(SRC)cpu.c $(INC)cpu.h
	$(CC) $(CFLAGS) -c -o $@ $<

display.o: $(SRC)display.c $(INC)display.h
	$(CC) $(CFLAGS) -c -o $@ $<

translator: translator.o

translator.o: $(SRC)translator.c $(INC)translator.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o
	mv $(ALL_EXECUTABLES) $(BIN)

.PHONY: clean all