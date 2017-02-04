#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/* need to build stream lib */
struct userPost {
	char* username;
	char* streamname;
	char* date;
	char* text;
};

class PostEntry {
	struct userPost post;
	char stream[256];
	char textArr[64][256];
	int numLine;
	void readInput(char* name)
	{
		post.username = name;
		printf("stream: ");
		fgets(stream, 255, stdin);
		int a = 0;
		while (stream[a++] != '\0')
			if (stream[a-1] == '\n')
				stream[a-1] = '\0';
		a=0;
		printf("text: ");
		while (a < 64 && fgets(textArr[a++], 255, stdin) != NULL);
		numLine = a-1;
		return;
	}
	void getTimeDate(char* timeString)
	{
		time_t timeT;
		struct tm* info;
		time(&timeT);
		info = localtime(&timeT);

		sprintf(timeString, "%d/%d/%d:%d/%d/%d", info->tm_sec, info->tm_min, info->tm_hour
			, info->tm_mday, info->tm_mon+1, info->tm_year+1900);
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
		for (a = 1; a < numLine; a++)
			strcat(text, textArr[a]);
		post.text = text;
		return;
	}
	int submitPost()
	{
		/* submit the post to the stream lib */
		printf("\nSubmitting post\n\n");
		printf("%s\n", post.username);
		printf("%s\n", post.streamname);
		printf("%s\n", post.date);
		printf("%s", post.text);

		free(post.text);
		free(post.date);
		return 0;
	}
};

int main(int argc, char* argv[])
{
	if (argc < 2)
		printf("No username\n");
	int a;
	char name[256];
	strcpy(name, argv[1]);
	for (a=2; a < argc; a++)
	{
		strcat(name, " ");
		strcat(name, argv[a]);
	}

	class PostEntry entry;
	entry.readInput(name);
	/*char curTime[256];
	entry.getTimeDate(curTime);*/
	entry.formatEntry();
	entry.submitPost();
	return 0;
}