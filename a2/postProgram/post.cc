#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
	char text[64][256];
	int numLine;
	void readInput()
	{
		
		printf("stream: ");
		fgets(stream, 255, stdin);
		int a = 0;
		while (a < 64 && fgets(text[a++], 255, stdin) != NULL);
		numLine = a-1;
		return;
	}
	void getTimeDate(char* time)
	{
		time = NULL;
	}
	void formatEntry()
	{
		/* put data into struct */
		char curTime[256];
		getTimeDate(curTime);
		post.date = curTime;
		return;
	}
	int submitPost()
	{
		/* submit the post to the stream lib */
		return 0;
	}
};

int main(int argc, char* argv[])
{
	if (argc < 1)
		printf("No username\n");
	int a;
	for (a=0; a < argc; a++)
		printf("%s\n", argv[a]);
	
	class PostEntry entry;
	entry.readInput();
	entry.formatEntry();
	/*char curTime[256];
	entry.getTimeDate(curTime);*/
	entry.formatEntry();
	entry.submitPost();
	return 0;
}