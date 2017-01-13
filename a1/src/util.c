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
		else if (tmp == '"')
		{
			char last = tmp;
			while (tmp != '"' && last != '\\')
			{
				last = tmp;
				append(line, tmp);
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

void parseFile(List* lines)
{
	int size = listSize(lines);
	int depth = 0;
	int firstFnc = 0;
	int tagged = 0;
	for (int a = 0; a < size; a++)
	{
		Data* d = (Data*)listGet(lines,a);
		//parse function here
		if (strcmp("class",d->line)==0)
		{
			free(d->line);
			char* tmp = malloc(sizeof(char)*7);
			strcpy(tmp, "struct");
			d->line = tmp;
			char * name = ((Data*)listGet(lines,a+1))->line;
			while (depth != 0 || strcmp("}",d->line)!=0)
			{
				d = (Data*)listGet(lines,++a);
				//locate first function
				if (strcmp("(",d->line)==0 && depth==1 && tagged==0)
				{
					firstFnc = a-2;
					tagged = 1;
				}
				//edit function names
				if (strcmp("(",d->line)==0 && depth==1)
				{
					Data* last = (Data*)listGet(lines,a-1);
					Data* type = (Data*)listGet(lines,a-2);
					char* oldName = last->line;
					char* newName = malloc(sizeof(char)*(strlen(oldName)+strlen(name)+1));
					strcpy(newName, name);
					strcat(newName,oldName);
					free(oldName);
					while (strcmp(")",d->line)!=0)
					{
						d = (Data*)listGet(lines,++a);
						if (strcmp(",",d->line)!=0 && strcmp(")",d->line)!=0)
						{
							newName = append(newName, d->line[0]);
							a++;
						}
					}
					char fncPtrString[256];
					strcpy(fncPtrString, "(*");
					strcat(fncPtrString, newName);
					strcat(fncPtrString, ")()")
					Data* ptrType = createLineSafe(type->line);
					Data* ptrName = createLineSafe(fncPtrString);
					Data* semi = createLineSafe(";");
					listInsert(lines, ptrType, firstFnc++);
					listInsert(lines, ptrName, firstFnc++);
					listInsert(lines, semi, firstFnc++);
					a+=3;
					last->line = newName;
				}

				if (strcmp("{",d->line)==0)
					depth++;
					
				if (strcmp("}",d->line)==0)
					depth--;
			}
			tagged = 0;
			Data* node = (Data*)listRemove(lines, a);
			listInsert(lines, node,firstFnc);
		}
	}
}

void printNewLine(int depth, List* lines, int index)
{
	printf("\n");
	int size = listSize(lines);
	char c = ' ';
	if (index+1 < size)
		c = ((Data*)listGet(lines,index+1))->line[0];
	if (c=='}')
		depth--;
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