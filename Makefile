CC = g++

all: crisscross formatter

crisscross: crisscross.o board.o word.o asciiformat.o
formatter: formatter.o board.o word.o asciiformat.o

clean:
	rm -f *.o crisscross