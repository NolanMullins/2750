OBJQ1 = stream.o list.o
FLAGS =-Wall -ansi -Iinclude -g -Istream
EXE = stream


all:$(OBJQ1) 
	gcc $(OBJQ1) -o $(EXE) $(FLAGS)

lib:$(OBJQ1) 
	ar rc stream.a $(OBJQ1) 

clean:
	rm $(OBJQ1) $(EXE)
run:
	./$(EXE)

test:
	gcc file.c 

%.o: src/%.c
	gcc -c src/$*.c $(FLAGS)

#stream.o: src/stream.c
#	gcc -c src/stream.c $(FLAGS)
#list.o: src/list.c include/list.h
#	gcc -c src/list.c $(FLAGS)
