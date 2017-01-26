#include <stdio.h> 
#include <stdlib.h> 
struct C { 
    int a; 
}; 
void constructorC ( struct C* myS ) { 
} 
struct mystruct { 
    float f; 
    double d; 
    long l; 
    short s; 
    int i; 
    char c; 
}; 
struct B { 
    float abc , cba; 
    struct mystruct* (*Bmethod) ( struct B* myStruct999 ); 
    void (*Bstring1c) ( char str[] , struct B* myStruct999 ); 
    void (*Bstring2c) ( char *str , struct B* myStruct999 ); 
}; 
struct mystruct* Bmethod ( struct B* myStruct999 ) { 
    struct mystruct *ptr; 
    int z = 3; 
    z ++; 
    printf ( "output should be 4 1.200000 : %d %f\n" , z , myStruct999->abc ); 
    ptr = malloc ( sizeof ( struct mystruct ) ); 
    return ( ptr ); 
}; 
void Bstring1c ( char str[] , struct B* myStruct999 ) { 
    printf ( "output should be abcd: %s\n" , str ); 
} 
void Bstring2c ( char *str , struct B* myStruct999 ) { 
    printf ( "output should be efgh: %s\n" , str ); 
} 
void constructorB ( struct B* myS ) { 
    myS->abc = 1.2; 
    myS->cba = 1; 
    myS->Bmethod = Bmethod; 
    myS->Bstring1c = Bstring1c; 
    myS->Bstring2c = Bstring2c; 
} 
/*this is a test
 * 
 */
// class
int main ( ) { 
    struct B myB; 
    constructorB(&myB); 
    printf ( "class" ); 
} 
