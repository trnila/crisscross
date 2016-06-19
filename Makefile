CC = g++

all: crisscross

crisscross: crisscross.o board.o word.o asciiformat.o

clean:
	rm -f *.o crisscross