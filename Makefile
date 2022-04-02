SRC = $(wildcard ./src/*.c)
INC = $(wildcard ./src/*.h)
OBJ = $(SRC:.c=.o)
CC = gcc
CFLAGS = -ggdb -g3
TGT = $(shell basename `pwd`)

$(TGT): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^
.PHONY: clean

clean:
	rm -f $(wildcard ./src/*.o) $(TGT) 
