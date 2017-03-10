#include <stdio.h>
#include <stdlib.h>

#include "structData.h"
#include "parse.h"
#include "list.h"
#include "gen.h"

void freeString(void* data)
{
	free((char*)data);
}

void desElement(void* data)
{
	Element* e = (Element*)data;
	e->data = listClear(e->data, freeString);
	free(e);
}

int main(int argc, char* argv[])
{
	List* list = parse("data/test.txt");
	/*int a;
	for (a = 0; a < listSize(list); a++)
	{
		Element* e = (Element*)listGet(list, a);
		printf("%c\n", e->tag);
		int b;
		for (b = 0; b < listSize(e->data); b++)
		{
			printf("%s\n", (char*)listGet(e->data, b));
		}
	}*/
	gen(list, "out.dat");
	list = listClear(list, desElement);
	return 0;
}
