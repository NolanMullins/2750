OBJ = main.o list.o parse.o gen.o
FLAGS = -Wall -Iinclude -ansi
EXE = a3

all: $(OBJ)
	gcc $(OBJ) $(FLAGS) -o $(EXE)

clean:
	rm $(OBJ) $(EXE)

%.o : %.c
	gcc -c $*.c $(FLAGS) -o $*.o