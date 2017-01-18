#include <stdio.h>
#include <list.h>
#include <string.h>
#include <util.h>

Data* createLine(char* data)
{
	Data* d = malloc(sizeof(Data));
	char* ptr = malloc(sizeof(char)*strlen(data)+1);
	strcpy(ptr, data);
	data[0] = '\0';
	d->line = ptr;
	return d;
}

void delLine(Data* data)
{
	free(data->line);
	free(data);
}

Data* createLineSafe(char* data)
{
	Data* d = malloc(sizeof(Data));
	char* ptr = malloc(sizeof(char)*strlen(data)+1);
	strcpy(ptr, data);
	d->line = ptr;
	return d;
}

char* append(char* line, char c)
{
	int len = strlen(line);
	line[len] = c;
	line[len+1] = '\0';
	return line;
}

List* convertToList(char* file, List* list)
{
	FILE* f = fopen(file, "r");
	char tmp = getc(f);
	char last = '0';
	int flag = 0;
	while (tmp != EOF)
	{
		char line[256];
		if (tmp == '\t' || tmp == ' ' || tmp == '\n' || tmp == '\r')
		{
			if (flag!=1)
			{
				listAdd(list, createLine(line));
				flag = 1;
			}
		}
		else if (tmp == ';' || tmp == '(' || tmp == ')' || tmp == '{' || tmp == '}')
		{
			if (flag == 0)
				listAdd(list, createLine(line));
			append(line, tmp);
			listAdd(list, createLine(line));
			flag = 1;
		}
		//parse in strings
		else if (tmp == '"')
		{
			char last = tmp;
			append(line, tmp);
			tmp = getc(f);
			while (tmp != '"' && last != '\\')
			{
				last = tmp;
				append(line, tmp);
				tmp = getc(f);
			}
			append(line, tmp);
			listAdd(list, createLine(line));
			last = '0';
		}
		else if (tmp == '/' && last == '/')
		{
			while (!(tmp == '\n' || tmp == '\r'))
			{
				append(line,tmp);
				last = tmp;
				tmp = getc(f);
			}
		}
		else if (tmp == '*' && last == '/')
		{
			while (!(tmp == '/' && last == '*'))
			{
				append(line,tmp);
				last = tmp;
				tmp = getc(f);
			}
			append(line, tmp);
		}
		else
		{
			flag = 0;
			//printf("%d\n", tmp);
			append(line, tmp);
		}
		last = tmp;
		tmp = getc(f);
	}
	fclose(f);
	return list;
} 

void replaceInList(List* list, char* remove, char* place)
{
	int size = listSize(list);
	for (int a = 0; a < size; a++)
	{
		Data* d = (Data*)listGet(list,a);
		if (strcmp(remove, d->line)==0)
		{
			free(d->line);
			char* ptr = malloc(sizeof(char)*strlen(place)+1);
			strcpy(ptr, place);
			d->line = ptr;
		}
	}
}

/****************************************** Functionality *******************************************/

char* createFncPtr(char* fncName)
{
	char* ptr = malloc(sizeof(char)*(strlen(fncName)+5+1));
	strcpy(ptr, "(*");
	strcat(ptr, fncName);
	strcat(ptr, ")()");
	return ptr;
}

List* genFncPtrs(List* functions)
{
	List* ptrs = init();
	for (int a = 0; a < listSize(functions); a++)
	{
		List* fnc = (List*)listGet(functions,a);
		List* fncPtr = init();
		if (listSize(fnc) < 2)
			continue;
		Data* type = (Data*)listGet(fnc, 0);
		Data* name = (Data*)listGet(fnc,1);
		char* fncPtrString = createFncPtr(name->line);
		//had to strcpy the string out else it got deleted for some reason
		char stringType[strlen(type->line)+1];
		strcpy(stringType, type->line);
		listAdd(fncPtr, createLine(stringType));
		listAdd(fncPtr, createLine(fncPtrString));
		//doing it direct seg faulted
		char tmp[2];
		strcpy(tmp, ";");
		listAdd(fncPtr, createLine(tmp));//createLine(";"));
		free(fncPtrString);
		listAdd(ptrs, fncPtr);
	}
	return ptrs;
}

char* getFncName(List* fnc, char* className)
{
	int a = 1;
	//printf("%d\n", listSize(fnc));
	Data* d = (Data*)listGet(fnc, a++);
	char* oldName = d->line;
	char* newName = malloc(sizeof(char)*(strlen(oldName)+strlen(className)+1));
	strcpy(newName, className);
	strcat(newName, oldName);
	//free(oldName);
	//append parameters to function name
	d = (Data*)listGet(fnc,a);
	while (strcmp(")",d->line)!=0)
	{
		d = (Data*)listGet(fnc,++a);
		if (strcmp(",",d->line)!=0 && strcmp(")",d->line)!=0)
		{
			newName = append(newName, d->line[0]);
			a++;
		}
	}
	return newName;
}

List* removeFnc(List* lines, int startIndex)
{
	int depth = 0;
	Data* d = (Data*)listGet(lines,startIndex);
	List* newList = init();
	//printf("CALL: %s\n", d->line);
	while (depth != 1 || strcmp("}",d->line)!=0)
	{
		//printf("%s\n", ((Data*)listRemove(lines, startIndex))->line);
		//printf("%s\n", tmp->line);
		//
		//listAdd(fnc, (Data*)listRemove(lines, startIndex));
		//printf("%s\n", ((Data*)listGet(fnc,listSize(fnc)-1))->line);
		if (strcmp("{",d->line)==0)
			depth++;
			
		if (strcmp("}",d->line)==0)
			depth--;
		Data* old = (Data*)listRemove(lines, startIndex);
		Data* newLine = createLine(old->line);
		listAdd(newList, newLine);
		delLine(old);

		//delLine(old);
		//printf("test2\n");
		//Data* tmp = createLine(old->line);
		d = (Data*)listGet(lines,startIndex);
		//printf("Line: %s depth:%d\n", d->line, depth);
	}
	//listAdd(fnc, (Data*)listRemove(lines, startIndex));
	Data* old = (Data*)listRemove(lines, startIndex);
	Data* tmp = createLine(old->line);
	listAdd(newList, tmp);
	delLine(old);
	//return fnc;
	return newList;
}

List* parseFunctions(List* lines, int a)
{
	List* functions = init();
	int depth = 0;
	char* className = ((Data*)listGet(lines,a+1))->line;
	Data* d = (Data*)listGet(lines,a);
	while (depth != 0 || strcmp("}",d->line)!=0)
	{
		if (strcmp("(",d->line)==0 && depth==1)
		{
			//Pull the function out
			List* myList = removeFnc(lines, a-2);
			char* fncName = getFncName(myList, className);
			Data* nameNode = (Data*)listGet(myList, 1);
			free(nameNode->line);
			nameNode->line = fncName;
			listAdd(functions, myList);
			//for (int a = 0; a < listSize(myList); a++)
				//printf("%s\n", ((Data*)listGet(myList,a))->line);
			a-=3;
		}
		d = (Data*)listGet(lines,++a);
		if (strcmp("{",d->line)==0)
			depth++;
		if (strcmp("}",d->line)==0)
			depth--;
	}
	return functions;
}

List* generateConstructor(List* functions, char* className)
{
	List* lines = init();
	listAdd(lines, createLineSafe("struct*"));
	char name[256];
	strcpy(name, "constructor");
	strcat(name, className);
	listAdd(lines, createLineSafe(name));
	char param[256];
	strcpy(param, "( struct ");
	strcat(param, className);
	strcat(param, "* myS )");
	listAdd(lines, createLineSafe(param));
	listAdd(lines, createLineSafe("{"));
	//add function pointers
	for (int a = 0; a < listSize(functions); a++)
	{

	}
	listAdd(lines, createLineSafe("}"));
	return lines;
}

void insertFunctions(List* lines, List* functions, int index)
{
	while (listSize(functions)>0)
	{
		List* fnc = (List*)listRemove(functions, 0);
		//printf("%s\n", ((Data*)listGet(fnc,0))->line);
		//function type not correctly inserted into list
		while (listSize(fnc)>0)
		{
			Data* line = (Data*)listRemove(fnc,0);
			listInsert(lines, line, index++);
		}
		delHead(fnc);
	}
}

void insertFunction(List* lines, List* function, int index)
{
	//printf("%s\n", ((Data*)listGet(fnc,0))->line);
	//function type not correctly inserted into list
	while (listSize(function)>0)
	{
		Data* line = (Data*)listRemove(function,0);
		listInsert(lines, line, index++);
	}
	delHead(function);
}

void parseFile(List* lines)
{
	//int size = listSize(lines);
	//int depth = 0;
	for (int a = 0; a < listSize(lines); a++)
	{
		Data* d = (Data*)listGet(lines,a);
		//parse function here
		if (strcmp("class",d->line)==0)
		{
			free(d->line);
			char* s = malloc(sizeof(char)*(strlen("struct")+1));
			strcpy(s,"struct");
			d->line = s;
			char* className = ((Data*)listGet(lines,a+1))->line;
			//parseFunction(lines, &a, &depth);
			//pull functions out of class
			List* functions = parseFunctions(lines, a);
			//Generate function pointers
			List* ptrs = genFncPtrs(functions);
			//gen contructor
			List* con = generateConstructor(functions, className);
			//move to end of class
			do 
			{
				d = (Data*)listGet(lines,++a);
			} while (strcmp(d->line, "}")!=0);
			//insert function pointers into struct
			insertFunctions(lines, ptrs, a);
			//move to the new end of the class
			do 
			{
				d = (Data*)listGet(lines,++a);
			} while (strcmp(d->line, "}")!=0);
			//insert the functions
			int sizeOfFunctions = 0;
			for (int i = 0; i < listSize(functions); i++)
			{
				List* fnc = (List*)listGet(functions, i);
				sizeOfFunctions += listSize(fnc);
			}
			insertFunctions(lines, functions, a+1);
			//calculate offset before passing functions in, move to location after funtions
			//generate constructer there
			a += sizeOfFunctions+1;
			int sizeOfCon = listSize(con);
			//insert constructor
			insertFunction(lines, con, a);
			a += sizeOfCon;
			//char tmp[256];
			//strcpy(tmp, "//constructer");
			//listInsert(lines, createLine(tmp), a++);
		}
	}
}

/******************************************* OUTPUT functions ***********************************************************/

void printNewLine(int depth, List* lines, int index)
{
	
	int size = listSize(lines);
	char c = ' ';
	if (index+1 < size)
		c = ((Data*)listGet(lines,index+1))->line[0];
	if (c=='}')
		depth--;
	if (c==';')
		if (((Data*)listGet(lines,index))->line[0]=='}')
			return;
	printf("\n");
	for (int a = 0; a < depth*4; a++)
				printf("%c", ' ');
}

void outputCode(List* lines)
{
	int size = listSize(lines);
	int depth = 0;
	for (int a = 0; a < size; a++)
	{
		Data* d = (Data*)listGet(lines,a);
		if (strlen(d->line)>1 && d->line[0] == '/' && d->line[1] == '/')
		{
			printf("%s", d->line);
			printNewLine(depth, lines, a);
			continue;
		}
		if (strlen(d->line)>1 && d->line[0] == '/' && d->line[1] == '*')
		{
			printf("%s", d->line);
			printNewLine(depth, lines, a);
			continue;
		}
		if (a < size-1 && strcmp(";",((Data*)listGet(lines,a+1))->line)==0)
			printf("%s", d->line);
		else
			printf("%s ", d->line);
		if (strcmp("}",d->line)==0)
			printNewLine(--depth, lines, a);
		if (strcmp("{",d->line)==0)
			printNewLine(++depth, lines, a);
		if (d->line[strlen(d->line)-1]=='>')
			printf("\n");
		if (strcmp(";",d->line)==0)
			printNewLine(depth, lines, a);
	}
}