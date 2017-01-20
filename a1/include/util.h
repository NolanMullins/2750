#ifndef __MULLINSN_UTIL__
#define __MULLINSN_UTIL__

typedef struct data
{
	char* line;
} Data;

List* convertToList(char* file, List* list);
void replaceInList(List* list, char* remove, char* place);
void outputCode(List* lines);
void parseFile(List* lines);
void delData(void* data);

#endif 