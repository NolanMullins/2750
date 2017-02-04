#################
# Nolan Mullins # 
#    2750 A2    #
#################
POST = post
FLAGS = -Wall -ansi -Iinclude -g
EXE = post

#Build and compile the c++ file
conv: echo postProgram/$(POST).cc runC2c
	gcc postProgram/$(POST).c $(FLAGS) -o $(EXE)
echo:
	@echo "\n*************************\n* Building Post program *\n*************************\n"
runC2c: buildC2c
	cp postProgram/$(POST).cc C2c/$(POST).cc -f \
	&& cd C2c && ./C2c $(POST).cc && make clean && rm $(POST).cc \
	&& cd .. && mv C2c/$(POST).c postProgram/$(POST).c

gcc:
	gcc postProgram/$(POST).c $(FLAGS) -o $(EXE)

buildC2c:
	cd C2c && make && cd ..

clean:
	rm postProgram/$(POST).c $(EXE)


	