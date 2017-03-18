#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stream.h"

/*int addUser(char* username, char* list);*/

int main(int argc, char* argv[])
{
	char streams[256], name[256];
	if (argc < 2)
	{
		printf("No name\n");
		exit(0);
	}
	int flag = strcmp("-r", argv[1]);
	int a = 0;
	if (flag==0)
		a++;
	*name = 0;
	if (argc < a+1)
	{
		printf("No name\n");
		exit(0);
	}
	strcpy(name, argv[++a]);
	if (argc < a+1)
	{
		printf("No streams\n");
		exit(0);
	}
	strcpy(streams, argv[++a]);
	/*while (++a < argc)
	{
		strcat(name, argv[a]);
		if (a+1 < argc)
			strcat(name, " ");
	}*/
	/*printf("%s\n", name);*/
	if (flag==0 && argc > 2)
		removeUser(name, streams);
	else
		addUser(name, streams);
	/*int sts = addUser();*/
	return 0;
}