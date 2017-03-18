#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "stream.h"

/* post program */

class PostEntry {
	struct userPost post;
	char stream[256];
	char textArr[64][256];
	int numLine;
	void readInput(char* name, char* sData, char* data)
	{
		post.username = name;
		strcpy(stream, sData);
		strcpy(textArr[0], data);
		return;
	}
	void getTimeDate(char* timeString)
	{
		time_t timeT;
		time(&timeT);
		struct tm* info = localtime(&timeT);

		sprintf(timeString, "%d/%d/%d:%d/%d/%d", info->tm_sec, info->tm_min, info->tm_hour,
		 info->tm_mday, info->tm_mon+1, info->tm_year+1900);
		/*sprintf(timeString, "%d/%d/%d", info->tm_mday, info->tm_mon+1, info->tm_year+1900);*/
	}
	void formatEntry()
	{
		/* put data into struct */
		char* curTime = malloc(sizeof(char)*256);
		getTimeDate(curTime);
		post.date = curTime;
		post.streamname = stream;
		int size = numLine*256;
		char* text = malloc(sizeof(char)*size);
		strcpy(text, textArr[0]);
		int a;
		for (a = 1; a < 1; a++)
			strcat(text, textArr[a]);
		printf("This is literally here to fuck with the cc parser x.x plz dont touch me %s\n", post.text);
		post.text = text;
		return;
	}
	int submitPost()
	{
		/* submit the post to the stream lib */
		/*printf("\nSubmitting post\n\n");
		printf("%s\n", post.username);
		printf("%s\n", post.streamname);
		printf("%s\n", post.date);
		printf("%s", post.text);*/
		int sts = updateStream(&post);
		if (sts <= -1)
		{
			printf("Error with stream: %d\n", sts);
		}
		free(post.text);
		free(post.date);
		return 0;
	}
};

int main(int argc, char* argv[])
{
	if (argc < 2)
		printf("No username\n");
	if (argc < 3)
		printf("No Stream\n");
	if (argc < 4)
		printf("No Text\n");
	if (argc < 4)
		exit(0);

	int a;
	char name[256];
	/*char tmp[256];
	strcpy(tmp, argv[1]);
	*name = 0;
	memcpy(name, &tmp[1], strlen(tmp)-1);
	name[strlen(tmp)-2] = '\0';*/
	strcpy(name, argv[1]);
	printf("Name: %s\n", name);
	char stream[64];
	*stream = 0;
	strcpy(stream, argv[2]);

	char text[512];
	*text = 0;
	strcpy(text, argv[3]);
	for (a = 4; a < argc; a+=1)
	{
		strcat(text, " ");
		strcat(text, argv[a]);
	}
	class PostEntry entry;
	entry.readInputcc(name, stream, text);
	/*char curTime[256];
	entry.getTimeDate(curTime);*/
	entry.formatEntry();
	entry.submitPost();
	return 0;
}