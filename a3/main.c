#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "structData.h"
#include "gen.h"
#include "parse.h"
#include "list.h"


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
	if (argc < 2)
		exit(0);
	List* list = parse(argv[1]);
	char user[64];
	char stream[64];
	int index=0, size=0, order=0;
	*user = 0;
	*stream = 0;
	int flag = 1;
	if (argc > 6)
	{
		if (strcmp(argv[2], "n")!=0)
			strcpy(user, argv[2]);
		if (strcmp(argv[3], "n")!=0)
			strcpy(stream, argv[3]);
		index = atoi(argv[4]);
		size = atoi(argv[5]);
		order = atoi(argv[6]);
	}
	else
	{
		flag = 0;
	}

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
	/*void gen(List* data, char* file, char* user, char* stream, int index, int size, int order, int flag)*/
	gen(list, user, stream, index, size, order, flag);
	list = listClear(list, desElement);
	return 0;
}
