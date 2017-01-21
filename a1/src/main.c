#include <stdio.h>
#include <list.h>
#include <util.h>
#include <string.h>

int main()
{
	List* myList = init();
	myList = convertToList("data/f3.dat", myList);
	//int size = listSize(myList);
	//for (int a = 0; a < listSize(myList); a++)
		//printf("'%s'\n", ((Data*)listGet(myList,a))->line);
	//replaceInList(myList, "class", "struct");
	parseFile(myList);

	outputCode(myList);

	listClear(myList, delData);

	//replaceInList(myList, "class", "struct");
	//outputCode(myList);
}