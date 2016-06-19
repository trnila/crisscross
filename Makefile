CC = g++

all: crisscross formatter

crisscross: crisscross.o board.o word.o asciiformat.o
formatter: formatter.o board.o word.o asciiformat.o

crisscross.pdf: all
	./crisscross | paps > crisscross.ps
	ps2pdf crisscross.ps



clean:
	rm -f *.o crisscross crisscross.ps