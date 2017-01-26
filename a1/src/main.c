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

	parseFile(myList);

	outputCode(myList, argv[1]);

	listClear(myList, delData);

	return 0;
}
