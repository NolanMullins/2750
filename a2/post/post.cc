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
	char input[256];
	void readInput()
	{
		/* read into input */
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
	class PostEntry entry;
	entry.readInput();
	entry.formatEntry();
	/*char curTime[256];
	entry.getTimeDate(curTime);*/
	entry.formatEntry();
	entry.submitPost();
	return 0;
}