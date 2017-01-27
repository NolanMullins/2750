#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
double vald; 
float valf; 
char valc; 
struct D myD; 
struct mystruct { 
    float f; 
    double d; 
    long l; 
    short s; 
    int i; 
    char c; 
}; 
/* function prototype */
float total ( float , float ); 
float product ( float , float ); 
/* class with overloaded methods */
struct A { 
    int a , aa; 
    int aaa; 
    /* add parameters */
    // add class variables
    void (*Aaddii) ( int a , int aa , struct A* myStruct999 ); 
    void (*Aadd) ( struct A* myStruct999 ); 
}; 
void Aaddii ( int a , int aa , struct A* myStruct999 ) { 
    printf ( "%d\n" , a + aa ); 
} 
void Aadd ( struct A* myStruct999 ) { 
    printf ( "%d\n" , myStruct999->a + myStruct999->aa ); 
} 
void constructorA ( struct A* myS ) { 
    myS->Aaddii = Aaddii; 
    myS->Aadd = Aadd; 
} 
// simple function outside of a class 
float product ( float a , float b ) { 
    return ( a *b ); 
} 
/* class with no methods */
struct B { 
    long b; 
    short c; 
}; 
void constructorB ( struct B* myS ) { 
} 
/* class with no variables */
struct C { 
    // create a class inside the method
    void (*Caddii) ( int a , int b , struct C* myStruct999 ); 
    void (*CmakeObject) ( struct C* myStruct999 ); 
}; 
void Caddii ( int a , int b , struct C* myStruct999 ) { 
    printf ( "%d\n" , a + b ); 
} 
void CmakeObject ( struct C* myStruct999 ) { 
    int a , b; 
    struct B myB1; 
    constructorB(&myB1); 
    a = 7; 
    myB1.b = 3; 
    myB1.c = 10; 
    printf ( "output should be 20: %d" , a + ( int ) myB1.b + myB1.c ); 
    /* call the method inside the class */
    printf ( "output should be 3: " ); 
    a = 1; 
    b = 2; 
    myStruct999->Caddii ( a , b ,  myStruct999 ); 
} 
void constructorC ( struct C* myS ) { 
    myS->Caddii = Caddii; 
    myS->CmakeObject = CmakeObject; 
} 
struct D { 
    int a , b , c; 
    void (*Dsetai) ( int val , struct D* myStruct999 ); 
    void (*Dsetbi) ( int val , struct D* myStruct999 ); 
    void (*Dsetci) ( int val , struct D* myStruct999 ); 
    int (*Dgeta) ( struct D* myStruct999 ); 
    int (*Dgetb) ( struct D* myStruct999 ); 
    int (*Dgetc) ( struct D* myStruct999 ); 
    void (*Dmath) ( struct D* myStruct999 ); 
}; 
void Dsetai ( int val , struct D* myStruct999 ) { 
    myStruct999->a = val; 
} 
void Dsetbi ( int val , struct D* myStruct999 ) { 
    myStruct999->b = val; 
} 
void Dsetci ( int val , struct D* myStruct999 ) { 
    myStruct999->c = val; 
} 
int Dgeta ( struct D* myStruct999 ) { 
    return ( myStruct999->a ); 
} 
int Dgetb ( struct D* myStruct999 ) { 
    return ( myStruct999->b ); 
} 
int Dgetc ( struct D* myStruct999 ) { 
    return ( myStruct999->c ); 
} 
void Dmath ( struct D* myStruct999 ) { 
    int result; 
    result = myStruct999->a + myStruct999->b *myStruct999->c / myStruct999->a; 
} 
void constructorD ( struct D* myS ) { 
    myS->Dsetai = Dsetai; 
    myS->Dsetbi = Dsetbi; 
    myS->Dsetci = Dsetci; 
    myS->Dgeta = Dgeta; 
    myS->Dgetb = Dgetb; 
    myS->Dgetc = Dgetc; 
    myS->Dmath = Dmath; 
} 
struct E { 
    float abc; 
    struct mystruct* (*Emethod) ( struct E* myStruct999 ); 
    void (*Estring1c) ( char str[] , struct E* myStruct999 ); 
    void (*Estring2c) ( char *str , struct E* myStruct999 ); 
}; 
struct mystruct* Emethod ( struct E* myStruct999 ) { 
    struct mystruct *ptr; 
    int z = 3; 
    z ++; 
    printf ( "output should be 4 1.200000 : %d %f\n" , z , myStruct999->abc ); 
    ptr = malloc ( sizeof ( struct mystruct ) ); 
    return ( ptr ); 
}; 
void Estring1c ( char str[] , struct E* myStruct999 ) { 
    printf ( "output should be abcd: %s\n" , str ); 
} 
void Estring2c ( char *str , struct E* myStruct999 ) { 
    printf ( "output should be efgh: %s\n" , str ); 
} 
void constructorE ( struct E* myS ) { 
    myS->abc = 1.2; 
    myS->Emethod = Emethod; 
    myS->Estring1c = Estring1c; 
    myS->Estring2c = Estring2c; 
} 
int main ( int argc , char *argv[] ) { 
    constructorD(&myD); 
    struct A myA; 
    constructorA(&myA); 
    struct B myB1 , myB2 , myB3; 
    constructorB(&myB3); 
    constructorB(&myB2); 
    constructorB(&myB1); 
    struct C myC; 
    constructorC(&myC); 
    int x , y; 
    char str[50]; 
    struct mystruct *sptr; 
    char *name; 
    struct E myE; 
    constructorE(&myE); 
    printf ( "Hello \" there\n" ); 
    strcpy ( str , "A string containing the word class may be tricky" ); 
    sptr = malloc ( sizeof ( struct mystruct ) ); 
    name = malloc ( sizeof ( char ) *100 ); 
    myA.a = 5; 
    myA.aa = 10; 
    printf ( "output should be 15: " ); 
    myA.Aadd ( &myA ); 
    x = 4; 
    y = 5; 
    printf ( "output should be 9: " ); 
    myA.Aaddii ( x , y , &myA ); 
    myB3.b = 10; 
    myB3.c = 11; 
    printf ( "output should be 10, 11: %ld %d" , myB3.b , myB3.c ); 
    myC.CmakeObject ( &myC ); 
    // comment with a keyword in it class A nota {
    x = 11; 
    myD.Dsetai ( x , &myD ); 
    printf ( "output should be 11: %d\n" , myD.Dgeta ( &myD ) ); 
    free ( sptr ); 
    sptr = myE.Emethod ( &myE ); 
    strcpy ( str , "abcd" ); 
    myE.Estring1c ( str , &myE ); 
    strcpy ( str , "efgh" ); 
    myE.Estring2c ( str , &myE ); 
} 
float total ( float a , float b ) { 
    return ( a + b ); 
} 
