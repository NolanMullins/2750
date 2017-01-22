#include <stdio.h>
#include <list.h>
#include <string.h>
#include <util.h>

char* strgen(char* data)
{
	char* ptr = malloc(sizeof(char)*(strlen(data)+1));
	strcpy(ptr, data);
	return ptr;
}

Data* createLine(char* data)
{
	Data* d = malloc(sizeof(Data));
	char* ptr = malloc(sizeof(char)*(strlen(data)+1));
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

void delData(void* data)
{
	free(((Data*)data)->line);
	free((Data*)data);
}

void freeString(void* data)
{
	free((char*)data);
}

Data* createLineSafe(char* data)
{
	Data* d = malloc(sizeof(Data));
	char* ptr = malloc(sizeof(char)*(strlen(data)+1));
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

char getNext(FILE* f)
{
	char next = getc(f);
	ungetc(next, f);
	return next;
}

int isOperator(char tmp)
{
	if (tmp == '=' || tmp == '<' || tmp == '>' || tmp == '+' || tmp == '-' || tmp == '/' || tmp == '*')
		return 1;
	return 0;
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
		else if (tmp == ';' || tmp == '(' || tmp == ')' || tmp == '{' || tmp == '}' || tmp == ',' || tmp == '.' || tmp == '&')
		{
			if (flag == 0)
				listAdd(list, createLine(line));
			append(line, tmp);
			listAdd(list, createLine(line));
			line[0] = '\0';
			flag = 1;
		}
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
		else if (tmp == '*' && last == '/')
		{
			while (!(tmp == '/' && last == '*'))
			{
				//printf("%c", tmp);
				append(line,tmp);
				last = tmp;
				tmp = getc(f);
			}
			append(line, tmp);
		}
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
		//parse in strings
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
			//line[0] = '\0';
			last = '0';
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

// returns 1 if a ( comes before a ) or ;
int checkForBrk(List* lines, int startIndex)
{
	Data* d = (Data*)listGet(lines, startIndex);
	do
	{
		d = (Data*)listGet(lines, startIndex++);
		if (strcmp("(", d->line) == 0)
			return 1;
		if (strcmp(")", d->line) == 0)
			return 0;
	} while (startIndex < listSize(lines) && strcmp(";", d->line));
	return 0;
}

//returns 1 if data type, 0 if not
int isDataType(char* string)
{
	char* types[] = {"unsigned", "char", "int", "short", "long", "float", "double", "void", "*", "signed"};
	for (int a = 0; a < 9; a++)
		if (strcmp(types[a], string)==0)
			return 1;
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
		if (isDataType(d->line) == 1 && strcmp(",",d->line)!=0 && strcmp(")",d->line)!=0)
		{
			newName = append(newName, d->line[0]);
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

char* getVarType(List* lines, int indexOfVar, char* varName)
{
	int depth = 0;
	char* type = malloc(sizeof(char)*21);
	type[0] = '\0';
	int index = 0;
	for (int a = indexOfVar-1; a > 0; a--)
	{
		Data* d = (Data*)listGet(lines, a);
		if (strcmp("{", d->line) == 0)
			depth--;
		if (strcmp("}", d->line) == 0)
			depth++;
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

char* getStructType(List* lines, int indexOfVar, char* varName)
{
	int depth = 0;
	char* type = malloc(sizeof(char)*256);
	type[0] = '\0';
	for (int a = indexOfVar-1; a > 0; a--)
	{
		Data* d = (Data*)listGet(lines, a);
		if (strcmp("{", d->line) == 0)
			depth--;
		if (strcmp("}", d->line) == 0)
			depth++;
		//case struct A myA;
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
		//case struct A myA, myA2;
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

//will run through a function and correct class initilization
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
		if (strcmp("class", d->line) == 0)
		{
			//NEED TO LOOP THIS IN ORDER TO ACCOUNT FOR struct A a1,a2;
			free(d->line);
			char* tmp = malloc(sizeof(char)*7);
			strcpy(tmp, "struct");
			d->line = tmp;

			Data* type = (Data*)listGet(function, index);
			Data* var = (Data*)listGet(function, index);;
			int refIndex = index;
			int isPtr = 0;
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

				//insert constuctor
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
		if ((strcmp(".", d->line) == 0 || strcmp("->", d->line) == 0) && checkForBrk(lines, index-1) == 1)
		{
			Data* name = (Data*)listGet(function, index-2);
			int c = index;
			int myDepth = 0;

			Data* fncName = (Data*)listGet(function, index);
			//edit function call to include struct type
			char* structType = getStructType(lines, index-2, name->line);
			char newFnc[256];
			strcpy(newFnc, structType);
			strcat(newFnc, fncName->line);
			free(fncName->line);
			fncName->line = strgen(newFnc);
			free(structType);

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
					//printf("NEED TO LOOK UP: '%s' Type: '%s'\n", tmp->line, getVarType(lines, c, tmp->line));
				}
			} while (strcmp(")", tmp->line) != 0 && depth != 0);
			char newLine[256];
			if (c-index > 2)
				strcpy(newLine, ", &");
			else
				strcpy(newLine, "&");
			strcat(newLine, name->line);
			listInsert(function, createLineSafe(newLine), c);
			
		}
	}
}

int checkEndVar(char* string)
{
	if (strcmp(";", string) == 0 || strcmp(")", string) == 0)
		return 1;
	if (strchr(string, '=') != NULL)
		return 1;
	return 0;
}

int checkFncForClassRef(List* function, List* classVars, char* className)
{
	int found[listSize(classVars)];
	for (int a = 0; a < listSize(classVars); a++)
		found[a] = 0;

	for(int a = 0; a < listSize(function); a++)
	{
		//find a data type
		while (++a < listSize(function) && isDataType(((Data*)listGet(function,a))->line) == 0)
		{
			for (int b = 0; b < listSize(classVars); b++)
			{
				Data* d = (Data*)listGet(function,a);
				if (found[b] == 0 && strcmp(d->line, (char*)listGet(classVars,b))==0)
				{
					//found a line referencing a class variable, need to edit the string
					char newVar[256];
					strcpy(newVar, "myStruct->");
					strcat(newVar, d->line);
					free(d->line);
					d->line = strgen(newVar);
				}
			}
		}
		//move through the typing to find the names
		a--;
		while (++a < listSize(function) && isDataType(((Data*)listGet(function,a))->line) == 1);
		a--;
		Data* myData;
		if (a+1 >= listSize(function))
			break;
		do
		{
			myData = (Data*)listGet(function,++a);
			if (checkEndVar(myData->line)==0 && strcmp(",", myData->line) != 0 && isDataType(myData->line) == 0)
				//printf("Var found: %s\n", myData->line);
				for (int b = 0; b < listSize(classVars); b++)
				{
					if (strcmp(myData->line, listGet(classVars,b))==0)
						found[b] = 1;
				}
		}
		while (a < listSize(function)-1 && checkEndVar(myData->line)==0);
		//Data* d = (Data*)listGet(function, a);
		//printf("__%s\n", d->line);
		
	}
	
	int a = 0, depth = 0;
	Data* d = (Data*)listGet(function,a);
	while (strcmp(")", d->line) != 0 || --depth != 0)
	{
		//if (strcmp(")", d->line) == 0) depth--;
		if (strcmp("(", d->line) == 0) depth++;
		d = (Data*)listGet(function,++a);
	}
	char param[256];
	if (a > 4)
		strcpy(param, ", struct ");
	else 
		strcpy(param, "struct ");
	strcat(param, className);
	strcat(param, "* myStruct");
	listInsert(function, createLineSafe(param), a);
	return 1;
}

List* identifyClassVars(List* lines, int start)
{
	//Need to identify variables and insert them into this list
	List* classVars = init();
	int i = start;
	while (i<listSize(lines) && strcmp("{", ((Data*)listGet(lines,i++))->line) != 0);
	//loop through variable lines
	while (i < listSize(lines) && isFunction(lines, ++i) == 0)
	{
		//looop through data type strings
		i--;
		while (++i < listSize(lines) && isDataType(((Data*)listGet(lines,i))->line) == 1);
		//read in var names
		Data* myData;
		i--;
		do
		{
			myData = (Data*)listGet(lines,++i);
			//printf("Looking at: '%s'\n", myData->line);
			if (!(strcmp(",", myData->line) == 0 || strcmp("=", myData->line) == 0 || strcmp(";", myData->line) == 0))
				//printf("Class %s Var found: %s\n", ((Data*)listGet(lines,a+1))->line,myData->line);
				listAdd(classVars, strgen(myData->line));
		}
		while (i < listSize(lines) && !(strcmp(";",myData->line) == 0 || strcmp("=", myData->line) == 0));
	}
	return classVars;
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
			//gather list of class vars
			List* classVars = identifyClassVars(lines, a);
			//after gathering functions, parse them looking for these vars and if so
			//add a struct className to the params

			free(d->line);
			char* s = malloc(sizeof(char)*(strlen("struct")+1));
			strcpy(s,"struct");
			d->line = s;
			char* className = ((Data*)listGet(lines,a+1))->line;
			//parseFunction(lines, &a, &depth);
			//pull functions out of class
			List* functions = parseFunctions(lines, a);

			//check for class var refs in functions
			for (int i = 0; i < listSize(functions); i++)
				checkFncForClassRef((List*)listGet(functions, i), classVars, className);

			//Generate function pointers
			List* ptrs = genFncPtrs(functions);
			//gen contructor
			List* con = generateConstructor(functions, className);
			//make edits to function
			for (int i = 0; i < listSize(functions); i++)
			{
				List* fnc = (List*)listGet(functions, i);
				functionProcessor(lines, fnc, 0);
			}
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
			a+=2; //account for };
			//insert the functions
			int sizeOfFunctions = 0;
			for (int i = 0; i < listSize(functions); i++)
			{
				List* fnc = (List*)listGet(functions, i);
				sizeOfFunctions += listSize(fnc);
			}
			insertFunctions(lines, functions, a++);
			//calculate offset before passing functions in, move to location after funtions
			//generate constructer there
			a += sizeOfFunctions-1;
			int sizeOfCon = listSize(con);
			//insert constructor
			insertFunction(lines, con, a);
			a += sizeOfCon;
			a-=1;
			//clear class vars list
			listClear(classVars, freeString);
		}
		else if (strcmp("(", d->line) == 0 && isFunction(lines, a) == 1)
		{
			//printf("Function found: %s\n", ((Data*)listGet(lines, a-1))->line);
			functionProcessor(lines, lines,a-2);
		}
	}
}

/******************************************* OUTPUT functions ***********************************************************/

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
	for (int a = 0; a < depth*4; a++)
				fprintf(f,"%c", ' ');
}

void outputCode(List* lines)
{
	int size = listSize(lines);
	int depth = 0;
	FILE* f = fopen("file.c", "w");
	for (int a = 0; a < size; a++)
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
}