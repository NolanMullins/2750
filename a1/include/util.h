#ifndef __MULLINSN_UTIL__
#define __MULLINSN_UTIL__

typedef struct data
{
	char* line;
} Data;

typedef struct class{
	char* name;
	char* param;
	List* structData;
	List* functions;
} Class;

List* convertToList(char* file, List* list);
void replaceInList(List* list, char* remove, char* place);
Class* parseClass(List* lines, int start, int end, char* class);

#endif 