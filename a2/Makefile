#################
# Nolan Mullins # 
#    2750 A2    #
#################
POST = post
FLAGS = -Wall -ansi -Iinclude -g
EXE = a2

#Build and compile the c++ file
conv: post/$(POST).cc runC2c
	gcc post/$(POST).c $(FLAGS) -o $(EXE)
runC2c: buildC2c
	cp post/$(POST).cc C2c/$(POST).cc -f \
	&& cd C2c && ./C2c $(POST).cc && make clean && rm $(POST).cc \
	&& cd .. && mv C2c/$(POST).c post/$(POST).c

buildC2c:
	cd C2c && make && cd ..

clean:
	rm post/$(POST).c $(EXE)
	
	