#include <stdio.h>
#include <list.h>
#include <util.h>
#include <string.h>

int main()
{
	List* myList = init();
	myList = convertToList("data/f.dat", myList);
	int size = listSize(myList);

	for (int a = 0; a < size; a++)
	{
		if (strcmp("class",((Data*)listGet(myList, a))->line)==0)
		{
			int start=0,end=0;
			int depth = 0;
			for (int b = a; b < size; b++)
			{
				if (strcmp("{",((Data*)listGet(myList, b))->line)==0)
					if (depth++==0)
						start = b+1;
				if (strcmp("}",((Data*)listGet(myList, b))->line)==0)
					if (--depth==0)
					{
						end = b-1;
						break;
					}
			}
			parseClass(myList, start, end, ((Data*)listGet(myList, a+1))->line);
		}
	}

	//replaceInList(myList, "class", "struct");
	for (int a = 0; a < size; a++)
	{
		//printf("%s\n", ((Data*)listGet(myList, a))->line);
	}
}