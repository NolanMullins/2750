#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mysql/mysql.h>

#define MAX_QUERY 512
#define HOSTNAME  "dursley.socs.uoguelph.ca"

#define USERNAME  "mullinsn"
#define PASSWORD  "0939720"
#define DATABASE  "mullinsn"

void error(char *msg, MYSQL *mysql){
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

void users(MYSQL* mysql)
{
	if (tableExists(mysql, "userData") == 0)
		return;

	MYSQL_RES *res;
	MYSQL_ROW row;
	char query[MAX_QUERY];
	
	*query = 0;

	strcpy(query, "SELECT userID FROM userData group by userID");

	if(mysql_query(mysql, query))
	  error("failed check ",mysql);

	if (!(res = mysql_store_result(mysql)))
		error("failed store 1",mysql);
	
	int numFields = mysql_num_fields(res);

	while ((row = mysql_fetch_row(res)))
	{
		int a;
		for(a = 0; a < numFields; a++) 
		{ 
			printf("%s ", row[a] ? row[a] : "NULL"); 
		} 
		printf("\n");
	}

	mysql_free_result(res); 
}

void posts(MYSQL* mysql)
{
	if (tableExists(mysql, "streamData") == 0)
		return;

	MYSQL_RES *res;
	MYSQL_ROW row;
	char query[MAX_QUERY];
	
	*query = 0;

	strcpy(query, "SELECT * FROM streamData order by streamID,ts ");

	if(mysql_query(mysql, query))
	  error("failed check ",mysql);

	if (!(res = mysql_store_result(mysql)))
		error("failed store 1",mysql);
	
	int numFields = mysql_num_fields(res);

	while ((row = mysql_fetch_row(res)))
	{
		int a;
		for(a = 1; a < numFields; a++) 
		{ 
			printf("%s ", row[a] ? row[a] : "NULL"); 
		} 
		printf("\n");
	}

	mysql_free_result(res); 
}

void streams(MYSQL* mysql)
{
	if (tableExists(mysql, "userData") == 0)
		return;

	MYSQL_RES *res;
	MYSQL_ROW row;
	char query[MAX_QUERY];
	
	*query = 0;

	strcpy(query, "SELECT streamID FROM userData group by streamID");

	if(mysql_query(mysql, query))
	  error("failed check ",mysql);

	if (!(res = mysql_store_result(mysql)))
		error("failed store 1",mysql);
	
	int numFields = mysql_num_fields(res);

	while ((row = mysql_fetch_row(res)))
	{
		int a;
		for(a = 0; a < numFields; a++) 
		{ 
			printf("%s ", row[a] ? row[a] : "NULL"); 
		} 
		printf("\n");
	}

	mysql_free_result(res); 
}

void clear(MYSQL* mysql)
{
	char query[MAX_QUERY];

	strcpy(query, "Truncate table userData");
	if (tableExists(mysql, "userData"))
		if(mysql_query(mysql, query))
	  		error("failed to clear userData ",mysql);

	strcpy(query, "Truncate table streamData");
	if (tableExists(mysql, "streamData"))
		if(mysql_query(mysql, query))
	  		error("failed to clear streamData ",mysql);

	printf("Success\n");
}

void reset(MYSQL* mysql)
{
	char query[MAX_QUERY];

	strcpy(query, "	DROP TABLE IF EXISTS userData");
	if(mysql_query(mysql, query))
	  error("failed to drop userData ",mysql);

	strcpy(query, "	DROP TABLE IF EXISTS streamData");
	if(mysql_query(mysql, query))
	  error("failed to drop streamData ",mysql);

	printf("Success\n");
}

void userTable(MYSQL* mysql)
{
	if (tableExists(mysql, "userData") == 0)
		return;

	MYSQL_RES *res;
	MYSQL_ROW row;
	char query[MAX_QUERY];
	
	*query = 0;

	strcpy(query, "SELECT * FROM userData order by userID ");

	if(mysql_query(mysql, query))
	  error("failed check ",mysql);

	if (!(res = mysql_store_result(mysql)))
		error("failed store 1",mysql);
	
	int numFields = mysql_num_fields(res);

	while ((row = mysql_fetch_row(res)))
	{
		int a;
		for(a = 1; a < numFields; a++) 
		{ 
			printf("%s ", row[a] ? row[a] : "NULL"); 
		} 
		printf("\n");
	}

	mysql_free_result(res); 
}

void postTable(MYSQL* mysql)
{
	if (tableExists(mysql, "streamData") == 0)
		return;

	MYSQL_RES *res;
	MYSQL_ROW row;
	char query[MAX_QUERY];
	
	*query = 0;

	strcpy(query, "SELECT * FROM streamData order by id ");

	if(mysql_query(mysql, query))
	  error("failed check ",mysql);

	if (!(res = mysql_store_result(mysql)))
		error("failed store 1",mysql);
	
	int numFields = mysql_num_fields(res);

	while ((row = mysql_fetch_row(res)))
	{
		int a;
		for(a = 0; a < numFields; a++) 
		{ 
			printf("%s ", row[a] ? row[a] : "NULL"); 
		} 
		printf("\n");
	}

	mysql_free_result(res); 
}


int main(int argc, char* argv[])
{
	MYSQL mysql;
	connectToDB(&mysql);

	int a;
	for (a = 1; a < argc; a++)
	{
		if (strcmp("-clear",argv[a])==0)
			clear(&mysql);
		else if (strcmp("-reset",argv[a])==0)
			reset(&mysql);
		else if (strcmp("-posts",argv[a])==0)
			posts(&mysql);
		else if (strcmp("-users",argv[a])==0)
			users(&mysql);
		else if (strcmp("-streams",argv[a])==0)
			streams(&mysql);
		else if (strcmp("-userData",argv[a])==0)
			userTable(&mysql);
		else if (strcmp("-postData",argv[a])==0)
			postTable(&mysql);
	}
	
	mysql_close(&mysql);
}