#include <stdio.h>
#include <string.h>
#include "structData.h"

int strcmpA3(char* literal, char* dataLine)
{
	int a;
	for (a = 0; a < strlen(dataLine); a++)
	{
		if (dataLine[a] == '=')
			return 1;
		if (a >= strlen(literal))
			return 0;
		if (dataLine[a] != literal[a])
			return 0;
	}
	return 0;
}

int indexOfChar(char* string, char hook)
{
	int a;
	for (a = 0; a < strlen(string); a++)
		if (string[a] == hook)
			return a;
	return -1;
}

char* getStr(List* list, int index)
{
	return (char*)listGet(list, index);
}

void bite(char* dest, char* src, int index)
{
	*dest = 0;
	int b;
	for (b=index; b < strlen(src); b++)
		dest[b-index] = src[b];
	dest[b-index] = '\0';
}



void genB(Element* e)
{
	List* args = e->data;
	int a;
	char name[256];
	char link[256];
	for (a=0; a < listSize(args); a++)
	{
		char* string = getStr(args, a);
		int equal = indexOfChar(string, '=')+1;
		if (strcmpA3("name", string))
			bite(name, string, equal);
		else if (strcmpA3("link", string))
			bite(link, string, equal);
	}
	printf("<form method=\"post\" action=%s><input type=\"submit\">%s</button></form>\n", link, name);
}

void genD(Element* e)
{
	printf("<hr>\n");
}

void genE(Element* e)
{

}

void genH(Element* e)
{
	List* args = e->data;
	int a;
	int size = 3;
	char text[256];
	strcpy(text, "HEADING");
	for (a=0; a < listSize(args); a++)
	{
		char* string = getStr(args, a);
		int equal = indexOfChar(string, '=')+1;
		if (strcmpA3("size", string))
		{
			size = 0;
			int b;
			for (b=equal; b < strlen(string); b++)
				size = size*10 + string[b]-'0';
		}
		else if (strcmpA3("text", string))
		{
			*text = 0;
			char tmp[256];
			bite(tmp, string, equal);
			memcpy(text, &tmp[1], strlen(tmp)-1);
			text[strlen(text)-1] = '\0';
		}
	}
	printf("<header><h%d>%s<h%d>\n", size, text, size);
}

void genI(Element* e)
{

}

void genL(Element* e)
{
	/*<a href="url">link text</a>*/
	List* args = e->data;
	int a;
	char url[256];
	char text[256];
	for (a=0; a < listSize(args); a++)
	{
		char* string = getStr(args, a);
		int equal = indexOfChar(string, '=')+1;
		if (strcmpA3("link", string))
			bite(url, string, equal);
		else if (strcmpA3("text", string))
		{
			*text = 0;
			char tmp[256];
			bite(tmp, string, equal);
			memcpy(text, &tmp[1], strlen(tmp)-1);
			text[strlen(text)-1] = '\0';
		}
	}
	printf("<a href=\"%s\">%s</a>\n", url, text);
}

void genP(Element* e)
{
	/*<img src="wrongname.gif" alt="HTML5 Icon" style="width:128px;height:128px;">*/
	List* args = e->data;
	int a;
	int width = 100, height = 100;
	char text[256];
	*text = 0;
	for (a=0; a < listSize(args); a++)
	{
		char* string = getStr(args, a);
		int equal = indexOfChar(string, '=')+1;
		if (strcmpA3("size", string))
		{
			width = 0;
			height = 0;
			int flag = 0;
			int b;
			for (b=equal; b < strlen(string); b++)
			{
				if (string[b] == 'x')
				{
					if (flag)
						break;
					flag = 1;
				}
				else if (flag)
					height = height*10+string[b]-'0';
				else
					width = width*10+string[b]-'0';
			}
		}
		else if (strcmpA3("image", string))
			bite(text, string, equal);
	}
	printf("<img src=%s alt=\"HTML5 Icon\" style=\"width:%dpx;height:%dpx;\">\n", text, width, height);
}

void genR(Element* e)
{

}

void genT(Element* e)
{

}

void gen(List* data, char* file)
{
	/*FILE* f = fopen(file, "w");*/
	int a;
	for (a = 0; a < listSize(data); a++)
	{
		Element* e = (Element*)listGet(data, a);
		switch (e->tag)
		{
			case 'b':
				genB(e);
			break;
			case 'd':
				genD(e);
			break;
			case 'e':
				genE(e);
			break;
			case 'h':
				genH(e);
			break;
			case 'i':
				genI(e);
			break;
			case 'l':
				genL(e);
			break;
			case 'p':
				genP(e);
			break;
			case 'r':
				genR(e);
			break;
			case 't':
				genT(e);
			break;
		}
	}

	/*fclose(f);*/
}