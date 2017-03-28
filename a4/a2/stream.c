#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "list.h"
#include "stream.h"

/* appends a char to a string*/
char* append(char* line, char c)
{
	int len = strlen(line);
	line[len] = c;
	line[len+1] = '\0';
	return line;
}

/* generates a malloced string */
char* strgen(char* data)
{
	char* ptr = malloc(sizeof(char)*(strlen(data)+1));
	strcpy(ptr, data);
	return ptr;
}

void freeString(void* data)
{
	free((char*)data);
}

int fileNotFound(char* fileName)
{
	FILE* f = fopen(fileName, "r");
	if (f == NULL)
		return 1;
	fclose(f);
	return 0;
}

int indexOfLastSpace(char* line)
{
	int len = strlen(line);
	int a;
	for (a=len-1; a>=0; a--)
		if (line[a] == ' ')
			return a;
	return -1;
}

int cmp(char* name, char* line)
{
	/* cmp */
	int lenN = strlen(name);/*, lenL = strlen(line);*/
	int sizeOfName = indexOfLastSpace(line);
	if (lenN == sizeOfName)
	{
		int a;
		for (a = 0; a < lenN; a++)
		{
			/*printf("%c:%c ", name[a], line[a]);*/
			if (name[a] != line[a])
				return 0;
		}
		return 1;
	}
	return 0;
}

int userExists(char* name, char* file)
{
	char tmp[256];
	strcpy(tmp, "messages/");
	strcat(tmp, file);
	strcat(tmp, "StreamUsers");
	FILE* f = fopen(tmp, "r");
	if (f == NULL)
		return 0;
	char line[256];
	while (fgets(line, 255, f) != NULL)
	{
		if (cmp(name, line))
		{
			fclose(f);
			return 1;
		}
	}
	fclose(f);
	return 0;
}

void createFile(char* fileName)
{
	FILE* f = fopen(fileName, "w");
	fprintf(f, "tmp");
	fclose(f);
}

int sizeOfPost(struct userPost *st)
{
	int numBytes = strlen(st->username);
	numBytes += strlen(st->date);
	numBytes += strlen(st->text);
	int a;
	int size = strlen(st->text);
	for (a = 0; a < size; a++)
		if (st->text[a] == '\n' || st->text[a] == '\r')
			numBytes--;
	return numBytes;
}

void printStruct(FILE* stream, FILE* data, struct userPost *st, int end)
{
	fprintf(stream, "%s\n", st->username);
	fprintf(stream, "%s\n", st->date);
	fprintf(stream, "%s\n", st->text);
	fprintf(data, "%d\n", end);
}

/* Lib functions */
int updateStream(struct userPost *st)
{
	/*cleanup*/
	if (st->streamname[strlen(st->streamname)-1] == '\n')
		st->streamname[strlen(st->streamname)-1] = '\0';

	char stream[256];
	char data[256];
	strcpy(stream, "messages/");
	strcat(stream, st->streamname);
	strcat(stream, "Stream");
	strcpy(data, "messages/");
	strcat(data, st->streamname);
	strcat(data, "StreamData");

	FILE *streamF, *dataF;
	if (userExists(st->username, st->streamname) == 0)
	{
		printf("User does not have access to stream: %s\n", st->streamname);
		return 2;
	}
	if (fileNotFound(stream))
	{
		streamF = fopen(stream, "w");
		dataF = fopen(data, "w");
		if (streamF == NULL)
			printf("err: %s\n", strerror(errno));
		printStruct(streamF, dataF, st,sizeOfPost(st));
		fclose(streamF);
		fclose(dataF);
		printf("Success\n");
		return 1;
	}
	streamF = fopen(stream, "a+");
	FILE* readBytes = fopen(data, "r");
	int numBytes = 0;
	char line[256];
	/* find number of lines in file */
	while(fgets(line, 255, readBytes) != NULL) 
	{
		if (line[strlen(line)-1] == '\n')
			line[strlen(line)-1] = '\0';
		numBytes=atoi(line);
	}
	fclose(readBytes);
	dataF = fopen(data, "a");
	/*printf("%d\n", numBytes);*/
	/* find number of lines in this post */
	int postBytes = sizeOfPost(st);
	/*printf("postSize: %d\n", postBytes);*/
	printStruct(streamF, dataF, st, numBytes+postBytes);
	
	fclose(streamF);
	fclose(dataF);
	printf("Success\n");
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
			strcpy(tmp, "messages/");
			strcat(tmp, stream);
			strcat(tmp, "StreamUsers");
			FILE* f;
			if (fileNotFound(tmp))
			{
				f = fopen(tmp, "w");
				if (f == NULL)
				{
					printf("err: %s\n", strerror(errno));
					exit(0);
				}
				char streamPath[256];
				char data[256];
				strcpy(streamPath, "messages/");
				strcat(streamPath, stream);
				strcat(streamPath, "Stream");
				strcpy(data, "messages/");
				strcat(data, stream);
				strcat(data, "StreamData");
				FILE* streamF = fopen(streamPath, "w");
				FILE* dataF = fopen(data, "w");
				if (streamF == NULL)
				{
					printf("err: %s\n", strerror(errno));
					exit(0);
				}
				printf("Creating stream: %s\n", stream);
				fclose(dataF);
				fclose(streamF);
			}
			else
				f = fopen(tmp, "a");
			if (userExists(username, stream))
			{
				printf("User already in stream: %s\n", stream);
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
	printf("Success\n");
	return 0;
}

int removeUser(char* username, char* list)
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
			strcpy(tmp, "messages/");
			strcat(tmp, stream);
			strcat(tmp, "StreamUsers");
			FILE* f;
			if (fileNotFound(tmp))
			{
				*stream = 0;
				if (list[a] == '\0')
					break;
				continue;
			}
			f = fopen(tmp, "r");
			/* if the user exists read in the file and remove their line */
			if (userExists(username, stream))
			{
				char line[256];
				List* lines = init();
				while (fgets(line, 255, f) != NULL)
				{
					if (strlen(line) > 0 && cmp(username, line) == 0)
					{
						listAdd(lines, strgen(line));
					}
				}
				fclose(f);
				f = fopen(tmp, "w");
				int size = listSize(lines);
				int b;
				for (b = 0; b < size; b++)
					if (((char*)listGet(lines, b))[0] != '\n')
						fprintf(f, "%s", (char*)listGet(lines, b));
				lines = listClear(lines, freeString);
				fclose(f);
			}
			else
				fclose(f);
			*stream = 0;
			if (list[a] == '\0')
				break;
		}
		else
			append(stream, list[a]);
		a++;
	}
	printf("Success\n");
	return 0;
}

/*int main()
{
	addUser("Nolan", "cats,dogs"); 
	addUser("Ryan", "cats,dogs"); 
	addUser("Rhys", "cats,dogs");
	struct userPost st;
	st.username = "Rhys";
	st.streamname = "cats";
	st.date = "todo";
	st.text = "hi\nryan";*
	int sts = updateStream(&st);
	if (sts == -1)
		printf("User Not Found\n");
	removeUser("Ryan", "cats");
	return 0;
}*/