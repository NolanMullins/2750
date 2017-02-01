#include <stdio.h>
#include <stdlib.h>

int add (float a, float b);

int add (int a, int b)
{
   return a+b;
}
float add (float a, float b)
{
   return a + b;
}
//tmp
int test (float a, float b);
/*this is a test
 * 
 */
 // class
int main()
{
	int a=1,b=1;
   float c=2,d=2;
   add(a,b);
   add(c,d);
   printf("%d %lf\n",add(a,b),add(c,d) );
}
