OBJQ1 = main.o list.o util.o
FLAGS =-Wall -ansi -Iinclude -g 
EXE = a1


all:$(OBJQ1) 
	gcc $(OBJQ1) -o $(EXE) $(FLAGS)
clean:
	rm $(OBJQ1) $(EXE) a.out
run:
	./$(EXE) data/sample3.cc

test:
	gcc file.c 

main.o: src/main.c
	gcc -c src/main.c $(FLAGS)
list.o: src/list.c
	gcc -c src/list.c $(FLAGS)
util.o: src/util.c
	gcc -c src/util.c $(FLAGS)
