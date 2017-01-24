#include <stdio.h>
#include <stdlib.h>

class C
{
	int a;
};

struct mystruct {
   float f;
   double d;
   long l;
   short s;
   int i;
   char c;
};

class
B {
   float
      abc = 1.2;

   struct mystruct *  method()
   {
   struct mystruct *ptr;
      int z = 3;
      z++;
      printf("output should be 4 1.200000 : %d %f\n", z, abc);

      ptr = malloc(sizeof(struct mystruct));
      return(ptr);
   };
   void string1(char str[]) {
      printf("output should be abcd: %s\n", str);
   }
   void string2(char *str) {
      printf("output should be efgh: %s\n", str);
   }
};

/*this is a test
 * 
 */
 // class
int main()
{
	class B myB;
	printf("class");
}
