/****************************************
 * Nolan Mullins
 * 0939720
 * 27/01/2017
 * 2750 - A1
 ****************************************/


    This program will take basic c++ code and convert it to to c code.
The program will identify key elements of c++ such as class' and method overloading
in order to edit the code to c. The program will also be able to handle scoping in classes
and link class variables to its methods after they get pulled out and linked to a struct.

Known issues
 - methods cant accept class variable types
     - ie int fnc (Class A b) {}
 - methods cant accept class variables as it messes with the naming
     - ie add (myC.a, myC.b) is a no go

***Instructions - compile***
type: make
output: a1

***Instructions - run***
type: ./a1 fileName.cc
output: filename.c
