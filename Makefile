CC = g++
CCFLAGS=-g
COMMON=board.o \
			 word.o \
			 asciiformat.o \
			 htmlformat.o

all: crisscross formatter

crisscross: crisscross.o $(COMMON)
formatter: formatter.o $(COMMON)

crisscross.pdf: all
	./crisscross | paps > crisscross.ps
	ps2pdf crisscross.ps



clean:
	rm -f *.o crisscross crisscross.ps
