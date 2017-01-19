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
	if (f == NULL)
		throwError("No File Found");
	char tmp = getc(f);
	char last = '0';
	int flag = 0;
	char line[512];
	line[0] = '\0';
	while (tmp != EOF)
	{
		if (tmp == '\t' || tmp == ' ' || tmp == '\n' || tmp == '\r')
		{
			if (flag!=1)
			{
				listAdd(list, createLine(line));
				line[0] = '\0';
				flag = 1;
			}
		}
		else if (tmp == ';' || tmp == '(' || tmp == ')' || tmp == '{' || tmp == '}')
		{
			if (flag == 0)
				listAdd(list, createLine(line));
			append(line, tmp);
			listAdd(list, createLine(line));
			line[0] = '\0';
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
			line[0] = '\0';
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

//return 1 if a class else 0 (Should be called after finding "class")
int isClass(List* lines, int startIndex)
{
	while (startIndex < listSize(lines))
	{
		Data* d = (Data*)listGet(lines, startIndex++);
		if (strcmp(d->line, "{") == 0)
			return 1;
		if (strcmp(d->line, ";") == 0)
			return 0;
	}
	return 0;
}

//returns 1 if function else 0
int isFunction(List* lines, int startIndex)
{
	while (startIndex < listSize(lines))
	{
		Data* d = (Data*)listGet(lines, startIndex++);
		if (strcmp(d->line, "{") == 0)
			return 1;
		if (strcmp(d->line, ";") == 0)
			return 0;
	}
	return 0;
}

char* createFncPtr(char* fncName)
{
	char* ptr = malloc(sizeof(char)*(strlen(fncName)+5+1));
	strcpy(ptr, "(*");
	strcat(ptr, fncName);
	strcat(ptr, ")");
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

		//char stringType[strlen(type->line)+1];
		//strcpy(stringType, type->line);
		listAdd(fncPtr, createLineSafe(type->line));
		listAdd(fncPtr, createLineSafe(fncPtrString));

		int c = 2;
		Data* d;
		int depth = 0;
		do
		{
			d = (Data*)listGet(fnc, c++);
			listAdd(fncPtr, createLineSafe(d->line));
			if (strcmp("(", d->line)==0)
				depth++;
			if (strcmp(")", d->line)==0)
				depth--;
		} while (strcmp(")",d->line)!=0 && depth != 0);

		listAdd(fncPtr, createLineSafe(";"));//createLine(";"));
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
	listAdd(lines, createLineSafe("void"));
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
		char ass[256];
		List* fnc = (List*)listGet(functions, a);
		char* fncName = ((Data*)listGet(fnc,1))->line;
		strcpy(ass, "myS->");
		strcat(ass,fncName);
		listAdd(lines, createLineSafe(ass));
		listAdd(lines, createLineSafe("="));
		listAdd(lines, createLineSafe(fncName));
		listAdd(lines, createLineSafe(";"));
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
	delHead(functions);
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

//will run through a function and correct class initilization
void functionProcessor(List* function, int start)
{
	int index = start;
	int depth = 0;
	while (start < listSize(function))
	{
		Data* d = (Data*)listGet(function, index++);
		if (strcmp("{", d->line) == 0)
			depth++;
		else if (strcmp("}", d->line) == 0)
			if (--depth == 0)
				return;
		if (strcmp("class", d->line) == 0)
		{
			free(d->line);
			char* tmp = malloc(sizeof(char)*7);
			strcpy(tmp, "struct");
			d->line = tmp;
			Data* type = (Data*)listGet(function, index);
			Data* var = (Data*)listGet(function, ++index);
			while(++index < listSize(function) && strcmp(";", ((Data*)listGet(function, index))->line) != 0);
			//insert constuctor
			char con[256];
			strcpy (con, "constructor");
			strcat(con, type->line);
			strcat(con, "(&");
			strcat(con, var->line);
			strcat(con, ")");
			listInsert(function, createLineSafe(con), ++index);
			listInsert(function, createLineSafe(";"), ++index);
		}
	}
}

void parseFile(List* lines)
{
	//int size = listSize(lines);
	//int depth = 0;
	for (int a = 0; a < listSize(lines); a++)
	{
		Data* d = (Data*)listGet(lines,a);
		//parse function here
		if (strcmp("class",d->line)==0 && isClass(lines, a) == 1)
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
		else if (strcmp("(", d->line) == 0 && isFunction(lines, a) == 1)
		{
			//printf("Function found: %s\n", ((Data*)listGet(lines, a-1))->line);
			functionProcessor(lines, a-2);
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