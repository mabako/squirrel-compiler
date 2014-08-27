CC=g++
CFLAGS=-Wall -O2 -I../squirrel/include
LDFLAGS=-L../squirrel/lib -lsquirrel -lsqstdlib
OBJ=sq-compiler.o

all: build

squirrel:
	cd ../squirrel && make

build: squirrel $(OBJ)
	$(CC) $(CFLAGS) -o sq-compiler $(OBJ) $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<
