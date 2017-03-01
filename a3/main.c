#include <stdio.h>
#include <stdlib.h>

#include "structData.h"
#include "parse.h"
#include "list.h"


int main(int argc, char* argv[])
{
	List* list = parse("data/file.dat");
	int a;
	for (a = 0; a < listSize(list); a++)
	{
		Element* e = (Element*)listGet(list, a);
		printf("%c\n", e->tag);
		int b;
		for (b = 0; b < listSize(e->data); b++)
		{
			printf("%s\n", (char*)listGet(e->data, b));
		}
	}
	return 0;
}