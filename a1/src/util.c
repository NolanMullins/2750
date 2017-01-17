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

//OLD
char* createFunctionName(List* lines, char* className, int* startIndex)
{
	int a  = *startIndex;
	Data* last = (Data*)listGet(lines,a-1);
	Data* d = (Data*)listGet(lines, a);
	char* oldName = last->line;
	char* newName = malloc(sizeof(char)*(strlen(oldName)+strlen(className)+1));
	strcpy(newName, className);
	strcat(newName,oldName);
	//free(oldName);
	//append parameters to function name
	while (strcmp(")",d->line)!=0)
	{
		d = (Data*)listGet(lines,++a);
		if (strcmp(",",d->line)!=0 && strcmp(")",d->line)!=0)
		{
			newName = append(newName, d->line[0]);
			a++;
		}
	}
	*startIndex = a;
	return newName;
}

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
		listAdd(fncPtr, createLine(type->line));
		listAdd(fncPtr, createLine(fncPtrString));
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

//OLD
void parseFunction(List* lines, int* startIndex, int* depth)
{
	int firstFnc = 0;
	int tagged = 0;
	int a = *startIndex;
	Data* d = (Data*)listGet(lines,a);
	free(d->line);
	char* tmp = malloc(sizeof(char)*7);
	strcpy(tmp, "struct");
	d->line = tmp;
	char * name = ((Data*)listGet(lines,a+1))->line;
	while ( a < listSize(lines) && (*depth != 0 || strcmp("}",d->line)!=0))
	{
		d = (Data*)listGet(lines,++a);
		//locate first function
		if (strcmp("(",d->line)==0 && *depth==1 && tagged==0)
		{
			firstFnc = a-2;
			tagged = 1;
		}
		//edit function names
		if (strcmp("(",d->line)==0 && *depth==1)
		{
			Data* last = (Data*)listGet(lines,a-1);
			Data* type = (Data*)listGet(lines,a-2);
			char* fncName = createFunctionName(lines, name, &a);
			//create function pointer
			char* fncPtrString = createFncPtr(fncName);
			listInsert(lines, createLineSafe(type->line), firstFnc++);
			listInsert(lines, createLineSafe(fncPtrString), firstFnc++);
			listInsert(lines, createLineSafe(";"), firstFnc++);
			free(fncPtrString);
			free(last->line);
			a+=3;
			last->line = fncName;
		}

		if (strcmp("{",d->line)==0)
			(*depth)++;
			
		if (strcmp("}",d->line)==0)
			(*depth)--;
	}

	tagged = 0;
	Data* node = (Data*)listRemove(lines, a);
	Data* semi = createLineSafe(";");
	listInsert(lines, node,firstFnc);
	listInsert(lines, semi,firstFnc+1);
	a++;
	*startIndex = a;
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
			//parseFunction(lines, &a, &depth);
			//pull functions out of class
			List* functions = parseFunctions(lines, a);
			//Generate function pointers
			List* ptrs = genFncPtrs(functions);
			//move to end of class
			do 
			{
				d = (Data*)listGet(lines,++a);
			} while (strcmp(d->line, "}")!=0);
			//insert function pointers into struct
			insertFunctions(lines, ptrs, a);
			do 
			{
				d = (Data*)listGet(lines,++a);
			} while (strcmp(d->line, "}")!=0);
			insertFunctions(lines, functions, a+1);
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