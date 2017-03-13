OBJ = main.o list.o parse.o gen.o
FLAGS = -Wall -Iinclude -ansi
EXE = a3

all: buildA2 $(OBJ)
	gcc $(OBJ) $(FLAGS) -o $(EXE)

buildA2:
	cd a2 && make && cd ..

clean: cleanA2
	rm $(OBJ) $(EXE)

cleanA2:
	cd a2 && make clean && cd ..
	
%.o : %.c
	gcc -c $*.c $(FLAGS) -o $*.o

push:
	git add -A .
	git commit -m "-"
	git push