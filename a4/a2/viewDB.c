#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mysql/mysql.h>

#define MAX_QUERY 512
#define HOSTNAME  "dursley.socs.uoguelph.ca"

#define USERNAME  "mullinsn"
#define PASSWORD  "0939720"
#define DATABASE  "mullinsn"

void error(char *msg, MYSQL *mysql)
{
	printf("%s\n%s\n",msg,mysql_error(mysql));
	exit(1);
}

void connectToDB(MYSQL* mysql)
{
	mysql_init(mysql);
	mysql_options(mysql, MYSQL_READ_DEFAULT_GROUP, "mydb");
	if (!mysql_real_connect(mysql, HOSTNAME, USERNAME, PASSWORD,DATABASE, 0, NULL, 0)) 
	{
	   error("Could not connect to host.",mysql);
	}
}

int tableExists(MYSQL* mysql, char* table)
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	char query[MAX_QUERY];
	
	*query = 0;

	strcpy(query, "SELECT count(*) FROM information_schema.tables WHERE table_schema = '");
	strcat(query, DATABASE);
	strcat(query, "' AND table_name = '");
	strcat(query, table);
	strcat(query, "'");

	if(mysql_query(mysql, query))
	  error("failed check ",mysql);

	if (!(res = mysql_store_result(mysql))){
		error("failed store 1",mysql);
	}

	row = mysql_fetch_row(res);
	mysql_free_result(res); 
	return atoi(row[0]);
}

void printRadioButtons(MYSQL* mysql, char* user, char* order)
{
	if (tableExists(mysql,  "userData") == 0)
		error("no user data",mysql);

	MYSQL_RES *res;
	MYSQL_ROW row;
	char query[MAX_QUERY];
	
	*query = 0;

	sprintf(query, "SELECT streamID FROM userData where userID = '%s' group by streamID", user);

	if(mysql_query(mysql, query))
	  error("failed check ",mysql);

	if (!(res = mysql_store_result(mysql)))
		error("failed store result set",mysql);

	printf("<html>\n<body>");
	printf("Pick a stream\n<hr>\n");
	printf("<form action=\"display.php\" method=\"post\">\n");

	printf("<input type=\"radio\" name=\"stream\" value=ALL checked> ALL<br>\n");

	while ((row = mysql_fetch_row(res)))
		printf("<input type=\"radio\" name=\"stream\" value=%s > %s<br>\n", row[0], row[0]);

	printf("<input type=\"submit\" value=\"Submit\"/>\n");
	printf("<input type=\"hidden\" name=\"user\" value=\"%s\">\n", user);
	printf("<input type=\"hidden\" name=\"streamChange\" value=\"1\">\n");
	printf("	<input type=\"hidden\" name=\"user\" value=\"%s\">\n", user);
	printf("	<input type=\"hidden\" name=\"index\" value=\"-1\">\n");
	printf("	<input type=\"hidden\" name=\"size\" value=\"-1\">\n");
	printf("	<input type=\"hidden\" name=\"order\" value=\"%s\">\n", order);
	printf("</form>");
	printf("<body>\n<html>\n");

	mysql_free_result(res); 
}

/*mysql_free_result(res);  */
MYSQL_RES* loadPosts(MYSQL* mysql, char* stream, char* user, char* size)
{
	MYSQL_RES *res;
	char query[MAX_QUERY];
	
	*query = 0;
	if (strcmp(stream, "All")==0)
		sprintf(query, "SELECT top %s * FROM streamData where streamID in (select streamID from userData where userID = '%s') order by ts", size, user);
	else
		sprintf(query, "SELECT top %s * FROM streamData where streamID = '%s' order by ts", size, stream);

	if(mysql_query(mysql, query))
	  error("failed loadPosts ",mysql);

	if (!(res = mysql_store_result(mysql)))
		error("failed store result set",mysql);

	return res;
}

void setRead(MYSQL* mysql, char* stream, char* user, int number)
{
	char query[MAX_QUERY];
	*query = 0;

	sprintf(query, "UPDATE userData SET postIndex = '%d' where userID = '%s' and streamID = '%s'", number, user, stream);

	if(mysql_query(mysql, query))
	  error("failed to update user data ",mysql);
}

void markAllRead(MYSQL* mysql, char* user, char* stream)
{
	MYSQL_ROW row;
	MYSQL_RES *res;
	char query[MAX_QUERY];
	
	*query = 0;
	if (strcmp(stream, "All")==0)
		sprintf(query, "SELECT streamID, count(ts) FROM streamData where streamID in (select streamID from userData where userID = '%s') group by streamID", user);
	else
		sprintf(query, "SELECT streamID, count(ts) FROM streamData where streamID = '%s' group by streamID", stream);

	if(mysql_query(mysql, query))
	  error("failed loadPosts ",mysql);

	if (!(res = mysql_store_result(mysql)))
		error("failed store result set",mysql);

	while ((row = mysql_fetch_row(res)))
		setRead(mysql, row[0], user, atoi(row[1])-1);

	mysql_free_result(res);

	printf("All messages in %s have been read\n", stream);
}

int main(int argc, char* argv[])
{
	MYSQL mysql;
	connectToDB(&mysql);

	if (strcmp("listStream",argv[1])==0)
	{
		/*exec('./a2/view.py listStream ' . $user . ' ' . $order . ' 2>&1', $index);*/
		printRadioButtons(&mysql, argv[2], argv[3]);
	}
	else if (strcmp("markAllRead",argv[1])==0)
	{
		/*exec('./a2/view.py markAllRead '.$user.' '.$stream.' '.$size.' 2>&1', $allReadInfo);*/
		markAllRead(&mysql, argv[2], argv[3]);
	}
	else if (strcmp("changeStream",argv[1])==0)
	{

	}
	else if (strcmp("nextPost",argv[1])==0)
	{

	}
	else if (strcmp("prevPost",argv[1])==0)
	{

	}
	
	mysql_close(&mysql);
	return 0;
}