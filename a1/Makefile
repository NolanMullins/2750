OBJQ1 = main.o
FLAGS = -Wall -std=c99 -Iinclude
EXE = run


all:$(OBJQ1) 
	gcc $(OBJQ1) -o $(EXE) $(FLAGS)
clean:
	rm $(OBJQ1) $(OBJQ2) $(EXE)
run:
	./$(EXE)

main.o: src/main.c
	gcc -c src/main.c $(FLAGS)
