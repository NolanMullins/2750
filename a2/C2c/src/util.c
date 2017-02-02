/* 
 * Nolan Mullins 0939720
 * assigment utilities for a1
 */

#include <stdio.h>
#include "list.h"
#include <string.h>
#include <util.h>

#define myStruct "myStruct999"

/* generates a malloced string */
char* strgen(char* data)
{
	char* ptr = malloc(sizeof(char)*(strlen(data)+1));
	strcpy(ptr, data);
	return ptr;
}

/* creates a data line and edits the passed in string */
Data* createLine(char* data)
{
	Data* d = malloc(sizeof(Data));
	char* ptr = malloc(sizeof(char)*(strlen(data)+1));
	strcpy(ptr, data);
	data[0] = '\0';
	d->line = ptr;
	return d;
}

/* deletes a line*/
void delLine(Data* data)
{
	free(data->line);
	free(data);
}

/* will del a Data* type var*/
void delData(void* data)
{
	free(((Data*)data)->line);
	free((Data*)data);
}

/* frees a string *used for passing in to linked list*/
void freeString(void* data)
{
	free((char*)data);
}

/* does the same as creates a line but does not edit the passed in string*/
Data* createLineSafe(char* data)
{
	Data* d = malloc(sizeof(Data));
	char* ptr = malloc(sizeof(char)*(strlen(data)+1));
	strcpy(ptr, data);
	d->line = ptr;
	return d;
}

/* appends a char to a string*/
char* append(char* line, char c)
{
	int len = strlen(line);
	line[len] = c;
	line[len+1] = '\0';
	return line;
}

/* gets the next char ina file without editing the buffer*/
char getNext(FILE* f)
{
	char next = getc(f);
	ungetc(next, f);
	return next;
}

/* checks is tmp is an operator*/
int isOperator(char tmp)
{
	if (tmp == '=' || tmp == '<' || tmp == '>' || tmp == '+' || tmp == '-' || tmp == '/' || tmp == '*')
		return 1;
	return 0;
}

/* converts a file to a list*/
List* convertToList(char* file, List* list)
{
	FILE* f = fopen(file, "r");
	if (f == NULL)
	{
		delHead(list);
		throwError("No File Found");
	}
	char tmp = getc(f);
	char last = '0';
	int flag = 0;
	char line[512];
	line[0] = '\0';
	while (tmp != EOF)
	{
		/* white space = new line*/
		if (tmp == '\t' || tmp == ' ' || tmp == '\n' || tmp == '\r')
		{
			if (flag!=1)
			{
				listAdd(list, createLine(line));
				line[0] = '\0';
				flag = 1;
			}
		}
		/* these marks = new line*/
		else if (tmp == ';' || tmp == '(' || tmp == ')' || tmp == '{' || tmp == '}' || tmp == ',' || tmp == '.' || tmp == '&')
		{
			if (flag == 0)
				listAdd(list, createLine(line));
			append(line, tmp);
			listAdd(list, createLine(line));
			line[0] = '\0';
			flag = 1;
		}
		/* read in a comment */
		else if (tmp == '/' && last == '/')
		{
			while (!(tmp == '\n' || tmp == '\r'))
			{
				append(line,tmp);
				last = tmp;
				tmp = getc(f);
			}
			flag = 1;
			listAdd(list, createLine(line));
		}
		/* read in a comment */
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
		/* deal with ptrs */
		else if (!(tmp == '/' && (getNext(f) == '*' || getNext(f) == '/')) && isOperator(tmp) == 1)
		{
			if (strlen(line)>0)
				listAdd(list, createLine(line));
			append(line, tmp);
			if (getNext(f) != '*' && isOperator(getNext(f)) == 1)
				append(line, getc(f));
			listAdd(list, createLine(line));
			flag = 1;
		}
		/*parse in strings*/
		else if (tmp == '"')
		{
			char last = tmp;
			append(line, tmp);
			tmp = getc(f);
			while (!(tmp == '"' && last != '\\'))
			{
				last = tmp;
				append(line, tmp);
				tmp = getc(f);
			}
			append(line, tmp);
			listAdd(list, createLine(line));
			last = '0';
		}
		else
		{
			flag = 0;
			append(line, tmp);
		}
		last = tmp;
		tmp = getc(f);
	}
	fclose(f);
	return list;
} 

/* search through a list and replace remove with place strings*/
void replaceInList(List* list, char* remove, char* place)
{
	int size = listSize(list);
	int a;
	for (a = 0; a < size; a++)
	{
		Data* d = (Data*)listGet(list,a);
		if (strcmp(remove, d->line)==0)
		{
			free(d->line);
			char* ptr = malloc(sizeof(char)*(strlen(place)+1));
			strcpy(ptr, place);
			d->line = ptr;
		}
	}
}

/****************************************** Functionality *******************************************/

/*return 1 if a class else 0 (Should be called after finding "class")*/
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

/*returns 1 if function else 0*/
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

/*returns 1 if a ( comes before a ) or ;*/
int checkForBrk(List* lines, int startIndex)
{
	Data* d = (Data*)listGet(lines, startIndex);
	do
	{
		d = (Data*)listGet(lines, startIndex++);
		if (strcmp("(", d->line) == 0)
			return 1;
		if (strcmp(")", d->line) == 0 || strcmp("{", d->line) == 0)
			return 0;
	} while (startIndex < listSize(lines) && strcmp(";", d->line));
	return 0;
}

/*returns 1 if data type, 0 if not*/
int isDataType(char* string)
{
	char* types[] = {"unsigned", "char", "int", "short", "long", "float", "double", "void", "*", "signed"};
	int a;
	for (a = 0; a < 9; a++)
		if (strcmp(types[a], string)==0)
			return 1;
	return 0;
}

/* takes a function name and returns a function pointer name*/
char* createFncPtr(char* fncName)
{
	char* ptr = malloc(sizeof(char)*(strlen(fncName)+6+1));
	strcpy(ptr, "(*");
	strcat(ptr, fncName);
	strcat(ptr, ")");
	return ptr;
}

/* generates a function pointer based on a passed function*/
List* genFncPtrs(List* functions)
{
	List* ptrs = init();
	int a;
	/* loop through the functions and generate fnc ptr declerations*/
	for (a = 0; a < listSize(functions); a++)
	{
		List* fnc = (List*)listGet(functions,a);
		List* fncPtr = init();
		if (listSize(fnc) < 2)
			continue;
		Data* type = (Data*)listGet(fnc, 0);
		Data* name = (Data*)listGet(fnc,1);
		char* fncPtrString = createFncPtr(name->line);

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

		listAdd(fncPtr, createLineSafe(";"));
		free(fncPtrString);
		listAdd(ptrs, fncPtr);
	}
	return ptrs;
}

/* generate the name of a function */
char* getFncName(List* fnc, char* className)
{
	int a = 1;
	Data* d = (Data*)listGet(fnc, a++);
	char* oldName = d->line;
	char* newName = malloc(sizeof(char)*(strlen(oldName)+strlen(className)+64));
	strcpy(newName, className);
	strcat(newName, oldName);
	/*append parameters to function name*/
	d = (Data*)listGet(fnc,a);
	while (strcmp(")",d->line)!=0)
	{
		d = (Data*)listGet(fnc,++a);
		if (isDataType(d->line) == 1 && strcmp(",",d->line)!=0 && strcmp(")",d->line)!=0 && strcmp("*",d->line)!=0)
		{
			newName = append(newName, d->line[0]);
		}
	}
	return newName;
}

/* generate the name of a function outside of a class*/
char* getFncName2(List* lines, int index)
{
	int a = index;
	Data* d = (Data*)listGet(lines, a++);
	char* oldName = d->line;
	char* newName = malloc(sizeof(char)*(strlen(oldName)+64));
	strcpy(newName, oldName);
	/*append parameters to function name*/
	d = (Data*)listGet(lines,a);
	while (strcmp(")",d->line)!=0)
	{
		d = (Data*)listGet(lines,++a);
		if (isDataType(d->line) == 1 && strcmp(",",d->line)!=0 && strcmp(")",d->line)!=0 && strcmp("*",d->line)!=0)
		{
			newName = append(newName, d->line[0]);
		}
	}
	return newName;
}

/* remove a function out of the main list and returns it in a seperate one*/
List* removeFnc(List* lines, int startIndex)
{
	int depth = 0;
	Data* d = (Data*)listGet(lines,startIndex);
	List* newList = init();
	/* loop while it hasnt hit the end of the function */
	while (depth != 1 || strcmp("}",d->line)!=0)
	{
		if (strcmp("{",d->line)==0)
			depth++;
			
		if (strcmp("}",d->line)==0)
			depth--;
		Data* old = (Data*)listRemove(lines, startIndex);
		Data* newLine = createLine(old->line);
		listAdd(newList, newLine);
		delLine(old);
		d = (Data*)listGet(lines,startIndex);
	}
	Data* old = (Data*)listRemove(lines, startIndex);
	Data* tmp = createLine(old->line);
	listAdd(newList, tmp);
	delLine(old);
	d = (Data*)listGet(lines,startIndex);
	/* account for the ; after the }*/
	if (strcmp(";", d->line) == 0)
	{
		old = (Data*)listRemove(lines, startIndex);
		tmp = createLine(old->line);
		listAdd(newList, tmp);
		delLine(old);
	}
	return newList;
}

/* parse functions in a class */
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
			/*Pull the function out*/
			Data* type = (Data*)listGet(lines, a-2);
			if (strcmp("*", type->line) == 0)
			{
				Data* prev = (Data*)listGet(lines, a-3);
				if (isDataType(prev->line) == 1)
				{
					Data* tmp = (Data*)listRemove(lines, a-3);
					char newType[256];
					strcpy(newType, tmp->line);
					delLine(tmp);
					strcat(newType, "*");
					free(type->line);
					type->line = strgen(newType);
					a--;
				}
				else if (strcmp("class", ((Data*)listGet(lines, a-4))->line) == 0 || strcmp("struct", ((Data*)listGet(lines, a-4))->line) == 0)
				{	
					/*merge data type info into one node*/
					Data* tmp = (Data*)listRemove(lines, a-4);
					Data* name = (Data*)listRemove(lines, a-4);
					Data* ptr = (Data*)listGet(lines, a-4);

					char newType[256];
					strcpy(newType, "struct ");
					strcat(newType, name->line);
					strcat(newType, ptr->line);
					free(ptr->line);
					ptr->line = strgen(newType);
					delLine(tmp);
					delLine(name);
					a-=2;
				}
			}
			List* myList = removeFnc(lines, a-2);
			char* fncName = getFncName(myList, className);
			Data* nameNode = (Data*)listGet(myList, 1);
			free(nameNode->line);
			nameNode->line = fncName;
			listAdd(functions, myList);
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

/* generates a contructor for a function*/
List* generateConstructor(List* functions, List* varsToAdd, char* className)
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

	int a;
	for (a = 0; a < listSize(varsToAdd); a++)
		listAdd(lines, createLineSafe(((Data*)listGet(varsToAdd, a))->line));
	/*add function pointers*/
	for (a = 0; a < listSize(functions); a++)
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

/*insert multiple functions into the main list ***will destroy the passed in function lists*/
void insertFunctions(List* lines, List* functions, int index)
{
	while (listSize(functions)>0)
	{
		List* fnc = (List*)listRemove(functions, 0);
		while (listSize(fnc)>0)
		{
			Data* line = (Data*)listRemove(fnc,0);
			listInsert(lines, line, index++);
		}
		delHead(fnc);
	}
	delHead(functions);
}
/*insert a single function into the main list ***will destroy the passed in function list*/
void insertFunction(List* lines, List* function, int index)
{
	/*function type not correctly inserted into list*/
	while (listSize(function)>0)
	{
		Data* line = (Data*)listRemove(function,0);
		listInsert(lines, line, index++);
	}
	delHead(function);
}

/* will search through a list look for the type of a passed in var*/
char* getVarType(List* lines, int indexOfVar, char* varName)
{
	int depth = 0;
	char* type = malloc(sizeof(char)*21);
	type[0] = '\0';
	int index = 0;
	int a;
	/* loop back from the position of the var*/
	for (a = indexOfVar-1; a > 0; a--)
	{
		Data* d = (Data*)listGet(lines, a);
		if (strcmp("{", d->line) == 0)
			depth--;
		if (strcmp("}", d->line) == 0)
			depth++;
		/* if it found a data type before the ref to the var in question */
		if (strcmp(varName, d->line) == 0 && isDataType(((Data*)listGet(lines, a-1))->line) == 1)
		{
			d = (Data*)listGet(lines, --a);
			while (isDataType(d->line) == 1)
			{
				if (strcmp("*", d->line) != 0)
					type[index++] = d->line[0];
				d = (Data*)listGet(lines, --a);
			}
			type[index] = '\0';
			return type;
		}
		/* if it found a multi decleration before the ref to the var in question */
		else if (strcmp(varName, d->line) == 0 && strcmp(",",((Data*)listGet(lines, a-1))->line) == 0)
		{
			int newA = a;
			Data* search = (Data*)listGet(lines, newA);
			do
			{
				search = (Data*)listGet(lines, --newA);
			} while (newA > 0 && !(isDataType(search->line) == 1 || strcmp("(", search->line) == 0));
			if (strcmp("(",search->line) != 0)
			{
				d = (Data*)listGet(lines, newA);
				while (isDataType(d->line) == 1)
				{
					if (strcmp("*", d->line) != 0)
						type[index++] = d->line[0];
					d = (Data*)listGet(lines, --newA);
				}
				type[index] = '\0';
				return type;
			}
		}
	}
	return type;
}

/* will return the type of struct */
char* getStructType(List* lines, int indexOfVar, char* varName)
{
	int depth = 0;
	char* type = malloc(sizeof(char)*256);
	type[0] = '\0';
	int a;
	for (a = indexOfVar-1; a > 0; a--)
	{
		Data* d = (Data*)listGet(lines, a);
		if (strcmp("{", d->line) == 0)
			depth--;
		if (strcmp("}", d->line) == 0)
			depth++;
		/*case struct A myA;*/
		if (strcmp(varName, d->line) == 0 && strcmp(",",((Data*)listGet(lines, a-1))->line) != 0)
		{
			int offset = 2;
			if (strcmp("*", ((Data*)listGet(lines, a-1))->line) == 0)
				offset++;
			if (strcmp("struct", ((Data*)listGet(lines, a-offset))->line) == 0)
			{
				d = (Data*)listGet(lines, a-offset+1);
				strcpy(type, d->line);
				return type;
			}
		}
		/*case struct A myA, myA2;*/
		else if (strcmp(varName, d->line) == 0 && strcmp(",",((Data*)listGet(lines, a-1))->line) == 0)
		{
			int newA = a;
			Data* search = (Data*)listGet(lines, newA);
			do
			{
				search = (Data*)listGet(lines, --newA);
			} while (newA > 0 && !(strcmp("struct", search->line) == 0 || strcmp("(", search->line) == 0));
			if (strcmp("(",search->line) != 0)
			{
				if (strcmp("struct", ((Data*)listGet(lines, newA))->line) == 0)
				{
					d = (Data*)listGet(lines, newA+1);
					strcpy(type, d->line);
					return type;
				}
			}
		}
	}
	return type;
}

/*will run through a function and correct class initilization*/
void functionProcessor(List* lines, List* function, int start)
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
		/* if a class has been found */
		if (strcmp("class", d->line) == 0)
		{
			
			free(d->line);
			char* tmp = malloc(sizeof(char)*10);
			strcpy(tmp, "struct");
			d->line = tmp;

			Data* type = (Data*)listGet(function, index);
			Data* var = (Data*)listGet(function, index);;
			int refIndex = index;
			int isPtr = 0;
			/* move through the decleration and take in data to gen the constructor */
			do
			{
				index = refIndex++;
				var = (Data*)listGet(function, refIndex);
				if (strcmp(",", var->line) == 0 || strcmp(";", var->line) == 0)
					continue;
				if (strcmp("*", var->line) == 0)
				{
					isPtr = 1;
					continue;
				}
				if (strcmp("=", var->line) == 0)
				{
					while(!(strcmp(",", var->line) == 0 || strcmp(";", var->line) == 0))
					{
						if (strcmp("class", var->line) == 0)
						{
							free(var->line);
							var->line = strgen("struct");
						}
						var = (Data*)listGet(function, ++refIndex);
					}
					if (!strcmp(";", var->line) == 0)
						continue;
				}

				while(++index < listSize(function) && strcmp(";", ((Data*)listGet(function, index))->line) != 0);
				
				if (strcmp(";", var->line) == 0)
					continue;

				/*insert constuctor*/
				char con[256];
				strcpy (con, "constructor");
				strcat(con, type->line);
				if (isPtr == 0)
					strcat(con, "(&");
				else
					strcat(con,"(");
				strcat(con, var->line);
				strcat(con, ")");
				listInsert(function, createLineSafe(con), ++index);
				listInsert(function, createLineSafe(";"), ++index);

				
			} while (strcmp(";", var->line) != 0);
		}
		/* found a class accessing a var or function */
		if ((strcmp(".", d->line) == 0 || strcmp("->", d->line) == 0) && checkForBrk(lines, index-1) == 1)
		{
			Data* name = (Data*)listGet(function, index-2);
			int c = index;
			int myDepth = 0;

			Data* fncName = (Data*)listGet(function, index);
			/*edit function call to include struct type*/
			char* structType = getStructType(lines, index-2, name->line);
			char newFnc[256];
			strcpy(newFnc, structType);
			strcat(newFnc, fncName->line);
			free(fncName->line);
			fncName->line = strgen(newFnc);

			Data* tmp = (Data*)listGet(function, c);
			do 
			{
				tmp = (Data*)listGet(function, ++c);
				if (strcmp("(", tmp->line) == 0)
					myDepth++;
				else if (strcmp(")", tmp->line) == 0)
					myDepth--;
				else if (strcmp(",", tmp->line) != 0 && strcmp("&", tmp->line) != 0 && strcmp(";", tmp->line) != 0)
				{
					char* type = getVarType(lines, c, tmp->line);
					if (strlen(structType) == 0)
					{
						free(type);
						continue;
					}
					char newName[256];
					strcpy(newName, fncName->line);
					strcat(newName, type);
					free(fncName->line);
					free(type);
					fncName->line = strgen(newName);
				}
			} while (strcmp(")", tmp->line) != 0 && myDepth != 0);
			char newLine[256];
			if (c-index > 2)
				strcpy(newLine, ", &");
			else
				strcpy(newLine, "&");
			strcat(newLine, name->line);
			listInsert(function, createLineSafe(newLine), c);
			free(structType);
		}
	}
}

/*if the string is an end variable, return 1 else 0*/
int checkEndVar(char* string)
{
	if (strcmp(";", string) == 0 || strcmp(")", string) == 0)
		return 1;
	if (strchr(string, '=') != NULL)
		return 1;
	return 0;
}

/* checks for a function referencing a class var*/
int checkFncForClassRef(List* function, List* classVars, char* className)
{
	int found[listSize(classVars)];
	int a;
	for (a = 0; a < listSize(classVars); a++)
		found[a] = 0;

	for(a = 0; a < listSize(function); a++)
	{
		/*find a data type*/
		while (++a < listSize(function) && (isDataType(((Data*)listGet(function,a))->line) == 0 || strcmp("*",((Data*)listGet(function,a))->line)==0))
		{
			int b;
			for (b = 0; b < listSize(classVars); b++)
			{
				Data* d = (Data*)listGet(function,a);
				if (found[b] == 0 && strcmp(d->line, (char*)listGet(classVars,b))==0)
				{
					/*found a line referencing a class variable, need to edit the string*/
					char newVar[256];
					strcpy(newVar, "myStruct999->");
					strcat(newVar, d->line);
					free(d->line);
					d->line = strgen(newVar);
				}
			}
		}
		/*move through the typing to find the names*/
		a--;
		while (++a < listSize(function) && isDataType(((Data*)listGet(function,a))->line) == 1 );
		a--;
		Data* myData;
		/*quick check for out of bounds*/
		if (a+1 >= listSize(function))
			break;
		/*go through all variables attached to typing and if they are a class variable mark them as found*/
		do
		{
			myData = (Data*)listGet(function,++a);
			if (checkEndVar(myData->line)==0 && strcmp(",", myData->line) != 0 && isDataType(myData->line) == 0)
			{
				int b;
				for (b = 0; b < listSize(classVars); b++)
				{
					if (strcmp(myData->line, listGet(classVars,b))==0)
						found[b] = 1;
				}
			}
		}
		while (a < listSize(function)-1 && checkEndVar(myData->line)==0);
		/*so the top loop can look at the next variable
		//plz trust this works, dont touch*/
		a--;
	}
	
	a = 0;
	int depth = 0;
	Data* d = (Data*)listGet(function,a);
	while (strcmp(")", d->line) != 0 || --depth != 0)
	{
		if (strcmp("(", d->line) == 0) depth++;
		d = (Data*)listGet(function,++a);
	}
	char param[256];
	if (a > 4)
		strcpy(param, ", struct ");
	else 
		strcpy(param, "struct ");
	strcat(param, className);
	strcat(param, "* myStruct999");
	listInsert(function, createLineSafe(param), a);
	return 1;
}

/* if it is a function returns 1 else 0 */
int isFncCall(List* fnc, int start)
{
	Data* d;
	int depth = 0;
	int a = start;
	do
	{
		d = (Data*)listGet(fnc, a);
		if (strcmp("(", d->line) == 0)
			depth++;
		else if (strcmp(")", d->line) == 0)
			depth--;
		else if (strcmp(";", d->line) == 0)
		{
			if (depth == 0)
				return 1;
			else 
				return 0;
		}
		else if (strcmp("{", d->line) == 0)
			return 0;
	} while (++a < listSize(fnc));
	return 0;
}

/* will look for local method refs in a class method */
void checkForClassMethodRef(List* lines, List* functions, List* function, char* className, int pos)
{
	Data* d;
	int a;
	for (a = 0; a < listSize(function); a++)
	{
		d = (Data*)listGet(function, a);
		/*initial check for a function call*/
		if (strcmp("(", d->line) == 0  && isFncCall(function, a) == 1)
		{
			Data* name = (Data*)listGet(function, a-1);
			int nameIndex= a-1;
			char newName[256];
			strcpy(newName, className);
			strcat(newName, name->line);
			Data* tmp = (Data*)listGet(function, a);
			/* attempt to create function name based on params */
			do
			{
				
				tmp = (Data*)listGet(function, a++);
				if (strcmp(",", tmp->line) != 0 && strcmp("&", tmp->line) != 0 && strcmp(";", tmp->line) != 0 && strcmp("(", tmp->line) != 0 && strcmp(")", tmp->line) != 0)
				{
					char* var = getVarType(function, a, tmp->line);
					if (strlen(var) == 0)
					{
						free(var);
						var = getVarType(lines, pos, tmp->line);
					}
					if (strlen(var) > 0)
						append(newName, var[0]);
					free(var);
				}
			} while (strcmp(";", tmp->line) != 0);
			int b;
			/* look to see if the function is a local fnc */
			for (b = 0; b < listSize(functions); b++)
			{
				List* myList = (List*)listGet(functions, b);
				Data* fnc = (Data*)listGet(myList, 1);
				if (strcmp(fnc->line, newName) == 0)
				{
					free(name->line);
					char newNew[256];
					strcpy(newNew, myStruct);
					strcat(newNew, "->");
					strcat(newNew, newName);
					name->line = strgen(newNew);
					/*insert function reference*/
					int depth = 0;
					int c = nameIndex;
					do
					{
						tmp = (Data*)listGet(function, c++);
						if (strcmp(")", tmp->line) == 0)
							depth--;
						else if (strcmp("(", tmp->line) == 0)
							depth++;
					} while (c < listSize(function) && !(strcmp(")", tmp->line) == 0 && depth == 0));
					listInsert(function, createLineSafe(myStruct), c-1);
					listInsert(function, createLineSafe(", "), c-1);
					break;
				}
			}
		}
	}
}

/* gens a list of a class var in a class */
List* identifyClassVars(List* lines, int start)
{
	/*Need to identify variables and insert them into this list*/
	List* classVars = init();
	int i = start;
	while (i<listSize(lines) && strcmp("{", ((Data*)listGet(lines,i++))->line) != 0);
	/*loop through variable lines*/
	while (i < listSize(lines) && isFunction(lines, ++i) == 0)
	{
		/*looop through data type strings*/
		i--;
		while (++i < listSize(lines) && isDataType(((Data*)listGet(lines,i))->line) == 1);
		/*read in var names*/
		Data* myData;
		i--;
		do
		{
			myData = (Data*)listGet(lines,++i);
			if (!(strcmp(",", myData->line) == 0 || strcmp("=", myData->line) == 0 || strcmp(";", myData->line) == 0))
				listAdd(classVars, strgen(myData->line));
		}
		while (i < listSize(lines) && !(strcmp(";",myData->line) == 0 || strcmp("=", myData->line) == 0));
	}
	return classVars;
}

/* looks for class variables being initialized upon decleration and fixes them */
List* getClassVarsInit(List* lines, int start)
{
	List* linesToAdd = init();
	/*Need to identify variables and insert them into this list*/
	int i = start;
	while (i<listSize(lines) && strcmp("{", ((Data*)listGet(lines,i++))->line) != 0);
	/*loop through variable lines*/
	while (i < listSize(lines) && isFunction(lines, ++i) == 0)
	{
		/*looop through data type strings*/
		i--;
		while (++i < listSize(lines) && isDataType(((Data*)listGet(lines,i))->line) == 1);
		/*read in var names*/
		Data* myData;
		i--;
		do
		{
			myData = (Data*)listGet(lines,++i);
			if (!(strcmp(",", myData->line) == 0 || strcmp("=", myData->line) == 0 || strcmp(";", myData->line) == 0))
			{
				/*copy var name and remove assignment statement*/
				if (strcmp("=", ((Data*)listGet(lines, i+1))->line) == 0)
				{
					char var[256];
					strcpy(var, "myS->");
					strcat(var, myData->line);
					listAdd(linesToAdd, createLineSafe(var));
					/*remove assignment statement*/
					myData = (Data*)listGet(lines,++i);
					while (strcmp(",", myData->line) != 0 && strcmp(";", myData->line) != 0)
					{
						listAdd(linesToAdd, listRemove(lines, i));
						myData = (Data*)listGet(lines,i);
					}
					listAdd(linesToAdd, createLineSafe(";"));
				}
			}
				
		}
		while (i < listSize(lines) && !(strcmp(";",myData->line) == 0 || strcmp("=", myData->line) == 0));
	}
	return linesToAdd;
}

/* will parse through a file and turn it from c++ to c */
void parseFile(List* lines)
{
	int a;
	int depth = 0;
	List* fncs = init();
	for (a = 0; a < listSize(lines); a++)
	{
		Data* d = (Data*)listGet(lines,a);
		if (strcmp("{", d->line) == 0)
			depth++;
		else if (strcmp("}", d->line) == 0)
			depth--;

		/*parse function here*/
		if (strcmp("class",d->line)==0 && isClass(lines, a) == 1)
		{
			/*gather list of class vars*/
			List* classVars = identifyClassVars(lines, a);
			/*Get variables init'd in a class scope*/
			List* addToCon = getClassVarsInit(lines, a);
			/*after gathering functions, parse them looking for these vars and if so
			 *add a struct className to the params*/
			free(d->line);
			d->line = strgen("struct");
			if (strcmp("*",((Data*)listGet(lines,a+1))->line) == 0)
				a++;
			char* className = ((Data*)listGet(lines,a+1))->line;
			
			/*pull functions out of class*/
			List* functions = parseFunctions(lines, a);

			/*Check if a method refs another local method in the class, also get the pos of the }*/
			int index = a;
			Data* tmp = (Data*)listGet(lines, index);
			do 
			{
				tmp = (Data*)listGet(lines, index++);
			} while (index < listSize(lines) && strcmp("}", tmp->line) != 0);
			int i;
			for (i = 0; i < listSize(functions); i++)
				checkForClassMethodRef(lines, functions ,(List*)listGet(functions, i), className, index);
			/*check for class var refs in functions*/
			for (i = 0; i < listSize(functions); i++)
				checkFncForClassRef((List*)listGet(functions, i), classVars, className);

			/*Generate function pointers*/
			List* ptrs = genFncPtrs(functions);
			/*gen contructor*/
			List* con = generateConstructor(functions, addToCon, className);
			/*make edits to function*/
			for (i = 0; i < listSize(functions); i++)
			{
				List* fnc = (List*)listGet(functions, i);
				functionProcessor(lines, fnc, 0);
			}
			/*move to end of class*/
			do 
			{
				d = (Data*)listGet(lines,++a);
			} while (strcmp(d->line, "}")!=0);
			/*insert function pointers into struct*/
			int numPtrs = listSize(ptrs);
			insertFunctions(lines, ptrs, a);
			/*move to the new end of the class*/
			if (numPtrs > 0)
			{
				do 
				{
					d = (Data*)listGet(lines,++a);
				} while (strcmp(d->line, "}")!=0);
				
			}
			a+=2; /*account for };*/
			/*insert the functions*/
			int sizeOfFunctions = 0;
			for (i = 0; i < listSize(functions); i++)
			{
				List* fnc = (List*)listGet(functions, i);
				sizeOfFunctions += listSize(fnc);
			}
			insertFunctions(lines, functions, a++);
			/*calculate offset before passing functions in, move to location after funtions
			 *generate constructer there*/
			a += sizeOfFunctions-1;
			int sizeOfCon = listSize(con);
			/*insert constructor*/
			insertFunction(lines, con, a);
			a += sizeOfCon;
			a-=1;
			/*clear class vars list*/
			listClear(classVars, freeString);
			/*clear other var list*/
			listClear(addToCon, delData);
		}
		else if (strcmp("class",d->line)==0)
		{
			/*swap class for struct*/
			free(d->line);
			d->line = strgen("struct");
			Data* type = (Data*)listGet(lines, ++a);
			Data* var = (Data*)listGet(lines, ++a);
			/*generate contructor and insert it into main*/
			int i = 0;
			do 
			{
				d = (Data*)listGet(lines, i++);
			} while (strcmp("main", d->line) != 0);
			do 
			{
				d = (Data*)listGet(lines, i++);
			} while (strcmp("{", d->line) != 0);
			/*gen contructor*/
			char con[256];
			strcpy (con, "constructor");
			strcat(con, type->line);
			strcat(con, "(&");
			strcat(con, var->line);
			strcat(con, ")");
			listInsert(lines, createLineSafe(con), i);
			listInsert(lines, createLineSafe(";"), i+1);
		}
		else if (strcmp("(", d->line) == 0 && isFunction(lines, a) == 1)
		{
			Data* name = (Data*)listGet(lines, a-1);
			if (strcmp("main", name->line) != 0)
			{
				char* newName = getFncName2(lines, a-1);
				free(name->line);
				name->line = newName;
				listAdd(fncs, strgen(newName));
			}
			functionProcessor(lines, lines,a-2);
		}
		/* function prototype*/
		else if (strcmp("(", d->line) == 0 && depth == 0)
		{
			Data* name = (Data*)listGet(lines, a-1);
			if (strcmp("main", name->line) != 0)
			{
				char* newName = getFncName2(lines, a-1);
				free(name->line);
				name->line = newName;
				listAdd(fncs, strgen(newName));
			}
		}
		/* function calls */
		else if (strcmp("(", d->line) == 0)
		{
			Data* name = (Data*)listGet(lines, a-1);
			if (strcmp("printf", name->line)==0)
				continue;
			char* newName = malloc(sizeof(char)*((strlen(name->line)+64)));
			strcpy(newName, name->line);
			/* attempt to create its new name */
			int tmpDepth = 0;
			while (!(strcmp(")", d->line) == 0 && tmpDepth == 0))
			{
				d = (Data*)listGet(lines, a++);
				if (strcmp(")", d->line) == 0)
					tmpDepth--;
				else if (strcmp("(", d->line) == 0)
					tmpDepth++;
				else if (strcmp(",", d->line) != 0)
				{
					char* tmp = getVarType(lines, a-1, d->line);
					strcat(newName, tmp);
					free(tmp);
				}
			}
			int i;
			/* check if the new name is in the list, if so replace the old name */
			for (i = 0; i < listSize(fncs); i++)
			{
				if (strcmp(newName, (char*)listGet(fncs,i)) == 0)
				{
					free(name->line);
					name->line = strgen(newName);
				}
			}
			free(newName);
		}	
	}
	listClear(fncs, freeString);
}

/******************************************* OUTPUT functions ***********************************************************/

/* prints a new line to a file with given indentation */
void printNewLineFile(FILE* f, int depth, List* lines, int index)
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
	fprintf(f,"\n");
	int a;
	for (a = 0; a < depth*4; a++)
				fprintf(f,"%c", ' ');
}

/* output code to filename -.cc +.c */
void outputCode(List* lines, char* filename)
{
	int size = listSize(lines);
	int depth = 0;
	char word[256];
	word[0] = '\0';
	int a = 0;
	/*gen file name*/
	while (filename[a] != '\0' && filename[a] != '.')
		append(word, filename[a++]);
	strcat(word, ".c");
	FILE* f = fopen(word, "w");
	for (a = 0; a < size; a++)
	{
		Data* d = (Data*)listGet(lines,a);
		if (strlen(d->line)>1 && d->line[0] == '/' && d->line[1] == '/')
		{
			fprintf(f,"%s", d->line);
			printNewLineFile(f,depth, lines, a);
			continue;
		}
		if (strlen(d->line)>1 && d->line[0] == '/' && d->line[1] == '*')
		{
			fprintf(f,"%s", d->line);
			printNewLineFile(f,depth, lines, a);
			continue;
		}
		if (strlen(d->line) > 1 && d->line[0] == '-' && d->line[1] == '>')
		{
			fprintf(f,"%s", d->line);
			continue;
		}
		/* FORMATING SLDGNLSGN */
		if (a < size-1 && strcmp(";",((Data*)listGet(lines,a+1))->line)==0)
			fprintf(f,"%s", d->line);
		else if (strcmp(d->line, "*") == 0)
			fprintf(f,"%s", d->line);
		else if (a < size-1 && strcmp(".",((Data*)listGet(lines,a+1))->line)==0)
			fprintf(f, "%s", d->line);
		else if (strcmp(".", d->line) == 0)
			fprintf(f, "%s", d->line);
		else if (a < size-1 && strcmp(">",((Data*)listGet(lines,a+1))->line)==0)
			fprintf(f, "%s", d->line);
		else if (strcmp("<", d->line) == 0)
			fprintf(f, "%s", d->line);
		else
			fprintf(f,"%s ", d->line);
		if (strcmp("}",d->line)==0)
			printNewLineFile(f,--depth, lines, a);
		if (strcmp("{",d->line)==0)
			printNewLineFile(f,++depth, lines, a);
		if (d->line[strlen(d->line)-1]=='>')
			fprintf(f,"\n");
		if (strcmp(";",d->line)==0)
			printNewLineFile(f,depth, lines, a);
	}

	fclose(f);
}