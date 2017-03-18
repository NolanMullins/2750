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

void printHiddenVals(char* user, char* stream, int index, int size, int order)
{
	if (strlen(user) > 0)
		printf("	<input type=\"hidden\" name=\"user\" value=\"%s\">\n", user);
	if (strlen(stream) > 0)
		printf("	<input type=\"hidden\" name=\"stream\" value=\"%s\">\n", stream);
	printf("	<input type=\"hidden\" name=\"index\" value=\"%d\">\n", index);
	printf("	<input type=\"hidden\" name=\"size\" value=\"%d\">\n", size);
	printf("	<input type=\"hidden\" name=\"order\" value=\"%d\">\n", order);
}

void genB(Element* e, char* user, char* stream, int index, int size, int order, int flag)
{
	List* args = e->data;
	int a;
	char name[256];
	char link[256];
	char hidden[256];
	char hVal[256];
	*hidden = 0;
	*hVal = 0;
	for (a=0; a < listSize(args); a++)
	{
		char* string = getStr(args, a);
		int equal = indexOfChar(string, '=')+1;
		if (strcmpA3("name", string))
			bite(name, string, equal);
		else if (strcmpA3("link", string))
			bite(link, string, equal);
		else if (strcmpA3("hidden", string))
			bite(hidden, string, equal);
		else if (strcmpA3("hVal", string))
			bite(hVal, string, equal);
	}
	printf("<form action=%s method=\"post\">\n", link);
	if (flag)
		printHiddenVals(user, stream, index, size, order);
	if (strlen(hidden) > 0)
		printf("	<input type=\"hidden\" name=%s value=%s>\n", hidden, hVal);
	printf("    <input type=\"submit\" value=%s/>\n",name);
	printf("</form>\n");
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
			text[strlen(tmp)-2] = '\0';
		}
	}
	printf("<h%d>%s</h%d>\n", size, text, size);
}

void genI(Element* e, char* user, char* stream, int index, int size, int order, int flag)
{
	/*
	<form action="/action_page.php">
	  input text <input type="text" name="fname"><br>
	  <input type="submit" value="Submit">
	</form>
	*/
	char actionPage[256];
	char text[256];
	char name[256];
	char value[256];
	int a;

	List* args = e->data;
	for (a=0; a < listSize(args); a++)
	{
		char* string = getStr(args, a);
		int equal = indexOfChar(string, '=')+1;
		if (strcmpA3("action", string))
		{
			bite(actionPage, string, equal);
			printf("<form action=%s method=\"post\">\n", actionPage);
			if (flag)
				printHiddenVals(user, stream, index, size, order);
		}
		else if (strcmpA3("text", string))
		{
			*text = 0;
			char tmp[256];
			bite(tmp, string, equal);
			memcpy(text, &tmp[1], strlen(tmp)-1);
			text[strlen(tmp)-2] = '\0';
		}
		/*{
			*text = 0;
			char tmp[256];
			bite(tmp, string, equal);
			memcpy(text, &tmp[1], strlen(tmp)-1);
			text[strlen(tmp)-2] = '\0';
		}*/
		else if (strcmpA3("name", string))
			bite(name, string, equal);
		else if (strcmpA3("value", string))
		{
			bite(value, string, equal);
			printf("    %s <input type=\"text\" name=%s value=%s><br>\n", text, name, value);
		}
	}
	printf("    <input type=\"submit\" value=\"click\">\n");
	printf("</form>\n");
}

void genL(Element* e)
{
	/*<a href="url">link text</a>*/
	List* args = e->data;
	int a;
	char url[256];
	char text[256];
	*text = 0;
	*url = 0;
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
			text[strlen(tmp)-2] = '\0';
		}
	}
	printf("<a href=%s>%s</a>\n", url, text);
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
			for (b=equal+1; b < strlen(string)-1; b++)
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

void genR(Element* e, char* user, char* stream, int index, int size, int order, int flag)
{
	/*
	<form>
	  <input type="radio" name="gender" value="male" checked> Male<br>
	  <input type="radio" name="gender" value="female"> Female<br>
	  <input type="radio" name="gender" value="other"> Other  
	</form>
	.r(action="radio.php",name="colour",value="red",value="green",value="blue")
	*/
	List* args = e->data;
	char action[64];
	char name[64];
	char values[16][64];
	int vlaueIndex = 0;
	int a;
	for (a=0; a < listSize(args); a++)
	{
		char* string = getStr(args, a);
		int equal = indexOfChar(string, '=')+1;
		if (strcmpA3("action", string))
			bite(action, string, equal);
		else if (strcmpA3("name", string))
			bite(name, string, equal);
		else if (strcmpA3("value", string))
		{
			*values[vlaueIndex] = 0;
			char tmp[256];
			bite(tmp, string, equal);
			memcpy(values[vlaueIndex] , &tmp[1], strlen(tmp)-1);
			values[vlaueIndex][strlen(tmp)-2] = '\0';
			++vlaueIndex;
		}
	}

	printf("<form action=%s>\n", action);
	if (flag)
		printHiddenVals(user, stream, index, size, order);
	printf("    <input type=\"radio\" name=%s value=\"%s\" checked> %s<br>\n", name, values[0], values[0]);
	for (a = 1; a < vlaueIndex; a++)
		printf("    <input type=\"radio\" name=%s value=\"%s\"> %s<br>\n", name, values[a], values[a]);
	printf("<input type=\"submit\" value=\"Submit\"/>\n");
	
	printf("</form>\n");
}

void genT(Element* e)
{
	List* args = e->data;
	int a;
	char file[64];
	char text[512];
	*text = 0;
	strcpy(text, "Default Text");
	for (a=0; a < listSize(args); a++)
	{
		char* string = getStr(args, a);
		int equal = indexOfChar(string, '=')+1;
		if (strcmpA3("text", string))
		{
			*text = 0;
			char tmp[256];
			bite(tmp, string, equal);
			memcpy(text, &tmp[1], strlen(tmp)-1);
			text[strlen(tmp)-2] = '\0';
		}
		else if (strcmpA3("file", string))
		{
			*file = 0;
			char tmp[256];
			bite(tmp, string, equal);
			memcpy(file, &tmp[1], strlen(tmp)-1);
			file[strlen(tmp)-2] = '\0';
			FILE* f = fopen(file, "r");
			if (f == NULL)
				continue;

			int index = 0;
			char tmpC = '0';
			while ((tmpC = getc(f)) != EOF)
			{
				if (tmpC == '\n')
				{
					text[index++] = '<';
					text[index++] = 'b';
					text[index++] = 'r';
					text[index++] = '>';
				}
				else
					text[index++] = tmpC;	
			}
			text[index++] = '\0';

			fclose(f);
		}
	}
	/*printf("<h5>");*/
	printf("%s\n", text);
	/*printf("<h5>");*/
}

void gen(List* data, char* user, char* stream, int index, int size, int order, int flag)
{
	printf("<html>\n<body>\n");
	/*printf("<br> User: %s stream: %s index: %d size: %d order: %d flag: %d <br>", user, stream, index, size, order, flag);*/
	
	int a;
	for (a = 0; a < listSize(data); a++)
	{
		Element* e = (Element*)listGet(data, a);
		printf("\n");
		switch (e->tag)
		{
			case 'b':
				genB(e, user, stream, index, size, order, flag);
			break;
			case 'd':
				genD(e);
			break;
			case 'e':
				/*genE(e);*/
			break;
			case 'h':
				genH(e);
			break;
			case 'i':
				genI(e, user, stream, index, size, order, flag);
			break;
			case 'l':
				genL(e);
			break;
			case 'p':
				genP(e);
			break;
			case 'r':
				genR(e, user, stream, index, size, order, flag);
			break;
			case 't':
				genT(e);
			break;
		}
	}
	printf("\n<body>\n<html>\n");

	for (a = 0; a < listSize(data); a++)
	{
		Element* e = (Element*)listGet(data, a);
		if (e->ta 'e')
		{
			/*execute cmd here*/
		}
	}
	/*fclose(f);*/
}