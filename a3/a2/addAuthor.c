#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stream.h"

/*int addUser(char* username, char* list);*/

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		printf("Need to enter a user\n");
		exit(0);
	}
	char streams[256], name[256];
	printf("list streams: ");
	fgets(streams, 255, stdin);
	if (streams[strlen(streams)-1] == '\n')
		streams[strlen(streams)-1] = '\0';
	if (strlen(streams) == 0)
		return 0;
	int flag = strcmp("-r", argv[1]);
	int a = 0;
	if (flag==0)
		a++;
	*name = 0;
	while (++a < argc)
	{
		strcat(name, argv[a]);
		if (a+1 < argc)
			strcat(name, " ");
	}
	/*printf("%s\n", name);*/
	if (flag==0 && argc > 2)
		removeUser(name, streams);
	else
		addUser(name, streams);
	/*int sts = addUser();*/
	return 0;
}