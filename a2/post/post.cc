#include <stdio.h>
#include <string.h>
#include <stdlib.h	>
class String
{
	char * cpy(char* old)
	{
		char* newS = malloc(sizeof(char)*(strlen(old)+1));
		strcpy(newS, old);
		return newS;
	}
};

int main(int argc, char* argv[])
{
	class String s;
	char* tmp = malloc(sizeof(char)*64);
	char* tmp2;
	strcpy(tmp, "test");
	tmp2 = s.cpy(tmp);
	printf("%s\n", tmp2);
	free(tmp);
	free(tmp2);
	return 0;
}