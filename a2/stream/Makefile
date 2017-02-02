OBJQ1 = stream.o list.o
FLAGS =-Wall -ansi -Iinclude -g 
EXE = stream


all:$(OBJQ1) 
	gcc $(OBJQ1) -o $(EXE) $(FLAGS)
clean:
	rm $(OBJQ1) $(EXE)
run:
	./$(EXE)

test:
	gcc file.c 

stream.o: src/stream.c
	gcc -c src/stream.c $(FLAGS)
list.o: src/list.c include/list.h
	gcc -c src/list.c $(FLAGS)
