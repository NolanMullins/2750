OBJQ1 = main.o list.o util.o
FLAGS =-Wall -ansi -Iinclude -g 
EXE = C2c


all:$(OBJQ1) 
	gcc $(OBJQ1) -o $(EXE) $(FLAGS)
clean:
	rm $(OBJQ1) $(EXE)
run:
	./$(EXE) data/sample3.cc

test:
	gcc file.c 

%.o: src/%.c
	gcc -c src/$*.c $(FLAGS)
#list.o: src/list.c
#	gcc -c src/list.c $(FLAGS)
#util.o: src/util.c
#	gcc -c src/util.c $(FLAGS)
