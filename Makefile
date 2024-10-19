CC = gcc
BIN = rocket
CFLAGS = -Wall -Wextra -pedantic -std=c99 -g
INC = -I./ -Iexternal/cutils
DEPS = raylib main.c rocket.o cvecs.o external/cutils/cdefs.h

INC_RAYLIB = -Lraylib/src -lraylib -Iraylib/src
.PHONY: raylib

all: $(DEPS)
	$(CC) -o $(BIN) main.c rocket.o cvecs.o $(CFLAGS) $(INC) $(INC_RAYLIB) -lm 

rocket.o: rocket.c rocket.h
	$(CC) -c rocket.c $(CFLAGS) $(INC) 

cvecs.o: external/cvecs/cvecs.c external/cvecs/cvecs.h
	$(CC) -c external/cvecs/cvecs.c $(CFLAGS) $(INC) 

raylib:
	cd raylib/src && $(MAKE) -j5

clean:
	rm -f $(BIN)
	rm -f *.o
	cd raylib/src && $(MAKE) clean
