OBJ = main.o list.o parse.o gen.o
FLAGS = -Wall -Iinclude -ansi
EXE = a3

all: buildDB buildA2 $(OBJ)
	gcc $(OBJ) $(FLAGS) -o $(EXE) && chmod 777 -R .

buildDB:
	gcc db.c -Iinclude -ansi -lmysqlclient -L/usr/lib/x86_64-linux-gnu/ -o  db

buildA2:
	cd a2 && make && cd ..

clean: cleanA2
	rm $(OBJ) $(EXE) db

cleanA2:
	cd a2 && make clean && cd ..
	
%.o : %.c
	gcc -c $*.c $(FLAGS) -o $*.o

push:
	git add -A .
	git commit -m "-"
	git push