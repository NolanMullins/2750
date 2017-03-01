OBJ = main.o list.o parse.o 
FLAGS = -Wall -Iinclude -ansi -Iinclude
EXE = run

all: $(OBJ)
	gcc $(OBJ) $(FLAGS) -o $(EXE)

clean:
	rm $(OBJ) $(EXE)

%.o : %.c
	gcc -c $*.c $(FLAGS) -o $*.o