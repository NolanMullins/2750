#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

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

int fileExists(char* fileName)
{
	FILE* f = fopen(fileName, "r");
	if (f == NULL)
		return 1;
	fclose(f);
	return 0;
}

int cmp(char* name, char* line)
{
	/* cmp */
	int lenN = strlen(name), lenL = strlen(line);
	if (lenN < lenL)
	{
		int a;
		for (a = 0; a < lenN; a++)
		{
			if (name[a] != line[a])
				return 0;
		}
		return 1;
	}
	return 0;
}

int userExists(char* name, char* file)
{
	FILE* f = fopen(file, "r");
	if (f == NULL)
		return 0;
	char line[256];
	while (fgets(line, 255, f) != NULL)
	{
		if (cmp(name, line))
			return 1;
	}
	return 0;
}

void createFile(char* fileName)
{
	FILE* f = fopen(fileName, "w");
	fprintf(f, "tmp");
	fclose(f);
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
			char tmp[256];
			strcpy(tmp, "users/");
			strcat(tmp, stream);
			FILE* f;
			if (fileExists(tmp))
			{
				f = fopen(tmp, "w");
				if (f == NULL)
					printf("err: %s\n", strerror(errno));
			}
			else
				f = fopen(tmp, "a");
			if (userExists(username, tmp))
			{
				printf("User already in file %s\n", stream);
			}
			else
			{
				fprintf(f, "%s 0\n", username);
			}
			
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
	addUser("Ryan", "cats,dogs");
	return 0;
}