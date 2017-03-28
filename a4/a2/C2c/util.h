/* Nolan Mullins 0939720
 * assigment utilities for a1
 */
#ifndef __MULLINSN_UTIL__
#define __MULLINSN_UTIL__

typedef struct data
{
	char* line;
} Data;

/* will take a file and convert it to a linked list */
List* convertToList(char* file, List* list);
/* Will replace string that = remove with place */
void replaceInList(List* list, char* remove, char* place);
/* will output formated code to filename from the passed in list*/
void outputCode(List* lines, char* filename);
/* will parse a file and edit the linked list, converting it from c++ to c */
void parseFile(List* lines);
/* free a data node */
void delData(void* data);

#endif 