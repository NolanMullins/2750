#################
# Nolan Mullins # 
#    2750 A2    #
#################
POST = post
FLAGS = -Wall -ansi -Iinclude -g -L. -lstream
EXE = post
AUTHOR = addAuthor.c
EXEAUTHOR = addauthor

all: echo lib conv author
	chmod +x view.py
	@echo "Done Building"

echo:
	@echo "\n*********************\n* Building programs *\n*********************\n"

author: $(AUTHOR)
	@echo "Building author program" ;\
	gcc $(AUTHOR) $(FLAGS) -o $(EXEAUTHOR)

#Build and compile the c++ file
conv: $(POST).cc runC2c
	@echo "Building post program" ;\
	gcc $(POST).c $(FLAGS) -o $(EXE)

runC2c: buildC2c
	cp $(POST).cc C2c/$(POST).cc -f ;\
	cd C2c && ./C2c $(POST).cc && make clean && rm $(POST).cc \
	&& cd .. && mv C2c/$(POST).c $(POST).c

gcc:
	gcc $(POST).c $(FLAGS) -o $(EXE)

buildC2c:
	cd C2c && make && cd ..

lib:
	cd stream && make lib ;\
	make clean ;\
	cd .. ;\
	mv stream/libstream.a libstream.a -f ;\
	cp stream/stream.h stream.h -f

clean:
	rm $(POST).c $(EXE) $(EXEAUTHOR) libstream.a stream.h

%.o : %.c
	gcc -c $*.c -o $*.o $(FLAGS)
	
