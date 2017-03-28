#include <stdio.h>
#include <string.h>

#include "structData.h"
#include "list.h"

/*removes spaces from a buffer util a different char is found*/
void removeSpace(FILE* f)
{
	char tmp = '0';
	while ((tmp = getc(f)) != EOF && tmp == ' ');
	ungetc(tmp, f);
}

/* appends a char to a string*/
char* append(char* line, char c)
{
	int len = strlen(line);
	line[len] = c;
	line[len+1] = '\0';
	return line;
}

/* generates a malloced string */
char* strgen(char* data)
{
	char* ptr = malloc(sizeof(char)*(strlen(data)+1));
	strcpy(ptr, data);
	return ptr;
}

Element* genElement(char tag, List* strings)
{
	Element* e = malloc(sizeof(Element));
	e->tag = tag;
	e->data = strings;
	return e;
}

List* parse(char* fileName)
{
	FILE* f = fopen(fileName, "r");
	if (f == NULL)
		throwError("No File Found");
	char tmp = '0';
	List* list = init();
	while ((tmp = getc(f)) != EOF)
	{
		if (tmp == '.')
		{
			tmp = getc(f);
			char tag = tmp;
			/*printf("%c\n", tmp);*/
			getc(f);
			removeSpace(f);
			int depth = 1;
			char data[256];
			List* strings = init();
			*data = 0;
			while ((tmp = getc(f)) != EOF)
			{
				if (tmp == '(')
					depth++;
				else if (tmp == ')')
				{
					depth--;
					if (depth <= 0)
					{
						listAdd(strings, strgen(data));
						listAdd(list, genElement(tag, strings));
						break;
					}
				}
				else if (tmp == ',')
				{
					listAdd(strings, strgen(data));
					/*printf("\n");*/
					*data = 0;
					removeSpace(f);
				}
				else
				{
					append(data, tmp);
					/*printf("%c", tmp);*/
				}
			}
			/*rintf("\n");*/
		}
	}

	fclose(f);
	return list;
}