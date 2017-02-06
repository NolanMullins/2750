#################
# Nolan Mullins # 
#    2750 A2    #
#################
POST = post
FLAGS = -Wall -ansi -Iinclude -g -L. -lstream
EXE = post
AUTHOR = author/addAuthor.c
EXEAUTHOR = addauthor

all: echo lib conv author
	@echo "Done Building"

echo:
	@echo "\n*********************\n* Building programs *\n*********************\n"

author: $(AUTHOR)
	@echo "Building author program" ;\
	gcc $(AUTHOR) $(FLAGS) -o $(EXEAUTHOR)

#Build and compile the c++ file
conv: postProgram/$(POST).cc runC2c
	@echo "Building post program" ;\
	gcc postProgram/$(POST).c $(FLAGS) -o $(EXE)

runC2c: buildC2c
	cp postProgram/$(POST).cc C2c/$(POST).cc -f ;\
	cd C2c && ./C2c $(POST).cc && make clean && rm $(POST).cc \
	&& cd .. && mv C2c/$(POST).c postProgram/$(POST).c

gcc:
	gcc postProgram/$(POST).c $(FLAGS) -o $(EXE)

buildC2c:
	cd C2c && make && cd ..

lib:
	cd stream && make lib ;\
	make clean ;\
	cd .. ;\
	mv stream/libstream.a libstream.a -f ;\
	cp stream/include/stream.h stream.h -f

clean:
	rm postProgram/$(POST).c $(EXE) $(EXEAUTHOR) libstream.a stream.h


	