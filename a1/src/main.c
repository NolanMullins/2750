#include <stdio.h>
#include <list.h>
#include <util.h>
#include <string.h>

int main(int argc, char* argv[])
{
	if (argc < 2)
		throwError("No file provided... exiting");
	List* myList = init();
	myList = convertToList(argv[1], myList);
	//int size = listSize(myList);
	//for (int a = 0; a < listSize(myList); a++)
		//printf("'%s'\n", ((Data*)listGet(myList,a))->line);
	//replaceInList(myList, "class", "struct");
	parseFile(myList);

	//replaceInList(myList, "class", "struct");

	outputCode(myList, argv[1]);

	listClear(myList, delData);

	//replaceInList(myList, "class", "struct");
	//outputCode(myList);
}
