APP = ./app
BIN = ./bin
INCLUDE = ./include
OBJ = ./obj
SRC = ./src

FLAGS = -O3 -Wall -std=gnu99

all: libed main

libed:
	gcc -c $(SRC)/hash.c -I $(INCLUDE) $(FLAGS) -o $(OBJ)/hash.o

main:
	gcc $(APP)/hashing_extensivel.c $(OBJ)/*.o -I $(INCLUDE) $(FLAGS) -o $(BIN)/hashing_extensivel -lm

run:
	$(BIN)/hashing_extensivel chaves.txt

clean:
	rm $(OBJ)/*.o
	rm $(BIN)/*
