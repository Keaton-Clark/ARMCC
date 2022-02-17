SRC = $(wildcard ./src/*.c)
INC = $(wildcard ./src/*.h)
OBJ = $(SRC:.c=.o)
CC = gcc
TGT = $(shell basename `pwd`)

$(TGT): $(OBJ) $(INC)
	$(CC) -ggdb -g3 -o $@ $^
.PHONY: clean

clean:
	rm -f $(wildcard ./src/*.o) $(TGT) 
