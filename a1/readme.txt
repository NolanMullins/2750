/****************************************
 * Nolan Mullins
 * 0939720
 * 27/01/2017
 * 2750 - A1
 ****************************************/


**********************************
Problem
**********************************
    This program will take basic c++ code and convert it to to c code.
The program will identify key elements of c++ such as class' and method overloading
in order to edit the code to c. The program will also be able to handle scoping in classes
and link class variables to its methods after they get pulled out and linked to a struct.


**********************************
Known Issues
**********************************
 - methods cant accept class variable types
     - ie int fnc (Class A b) {}
 - methods cant accept class variables as it messes with the naming
     - ie add (myC.a, myC.b) is a no go
 - placement of comments isnt 100% perfect but not that bad

**********************************
Guide
**********************************
Compile
type: make
output: a1

Running
type: ./a1 fileName.cc
output: filename.c

**********************************
Test plan
**********************************
tested various edge cases for each of the methods, program worked and performed as expected
for all cases listed below.
Overloading methods:			
    in a class, methods where stripped out and renamed correctly 
    	-except in case of passing in classes
    methods out of a class
    	- same as above

Passing in empty files
	Program exited with error and no mem leaks

Parsing classes
	Function pointers where generated
	Constructos linked functions and initilized variables

Scoping
	Declared a class in global scope
		class was init'd in main
	Methods accessing class variables had those vars linked to a struct

Memory
	Average usage 50k bytes
	0 leaks
	0 errors

Tested on cis2750 server
	- no differences where found

**********************************
Improvements
**********************************
Could clean up code and split it into more files, but it works. There are a few edge cases
that I didnt bother fixing as they would be very time consuming for a small piece