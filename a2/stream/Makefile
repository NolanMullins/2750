OBJQ1 = stream.o
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

stream.o: stream.c
	gcc -c stream.c $(FLAGS)
