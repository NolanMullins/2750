#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct userPost {
	char* username;
	char* streamname;
	char* date;
	char* text;
};

/* appends a char to a string*/
char* append(char* line, char c)
{
	int len = strlen(line);
	line[len] = c;
	line[len+1] = '\0';
	return line;
}


int updateStream(struct userPost *st)
{
	printf("Updating\n");
	return 0;
}

int addUser(char* username, char* list)
{
	char stream[256];
	int a = 0;
	stream[0] = '\0';
	while (1)
	{
		if (list[a] == ',' || list[a] == '\0')
		{
			/* list function */
			printf("%s\n", stream);
			char tmp[256];
			strcpy(tmp, "data/");
			strcat(tmp, stream);
			FILE* f = fopen(tmp, "a");
			fprintf(f, "%s 0\n", username);
			fclose(f);
			*stream = 0;
			if (list[a] == '\0')
				break;
		}
		else
			append(stream, list[a]);
		a++;
	}
	return 0;
}

int removeUser(char* username, char* list)
{
	printf("Removing\n");
	return 0;
}

int main()
{
	addUser("Rhys", "cats,dogs");
	return 0;
}