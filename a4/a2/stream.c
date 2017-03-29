#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "list.h"
#include "stream.h"

#include <mysql/mysql.h>

#define MAX_QUERY 512
#define HOSTNAME  "dursley.socs.uoguelph.ca"

#define USERNAME  "mullinsn"
#define PASSWORD  "0939720"
#define DATABASE  "mullinsn"

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

void error(char *msg, MYSQL *mysql){
	printf("%s\n%s\n",msg,mysql_error(mysql));
	exit(1);
}

void createSteamTable(MYSQL* mysql)
{
	char query[MAX_QUERY];	
	*query = 0;
	
	strcpy(query, "create table streamData (id int not null auto_increment,");
	strcat(query, "streamID char(64),");
	strcat(query, "userID char(64),");
	/*strcat(query, "date char(64),");*/
	strcat(query, "ts TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,");
	strcat(query, "data varchar(512),");
	strcat(query, "primary key(id) )");
	
	if(mysql_query(mysql, query)){
	  error("Could not create table",mysql);
	}
}

void createUserTable(MYSQL* mysql)
{
	char query[MAX_QUERY];
	*query = 0;
	
	strcpy(query, "create table userData (id int not null auto_increment,");
	strcat(query, "userID char(64),");
	strcat(query, "streamID char(64),");
	strcat(query, "postIndex int,");
	strcat(query, "primary key(id) )");	

	if(mysql_query(mysql, query)){
	  error("Could not create table",mysql);
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

int checkUserExists(MYSQL* mysql, char* user, char* stream)
{

	if (tableExists(mysql, "userData")==0)
		return 0;

	MYSQL_RES *res;
	MYSQL_ROW row;
	char query[MAX_QUERY];
	
	*query = 0;
	sprintf(query, "SELECT count(*) FROM userData WHERE streamID = '%s' and userID = '%s'", stream, user);

	if(mysql_query(mysql, query))
	  error("failed to check if user exists ",mysql);

	if (!(res = mysql_store_result(mysql))){
		error("failed store result",mysql);
	}

	row = mysql_fetch_row(res);
	mysql_free_result(res); 
	return atoi(row[0]);
}

void connectToDB(MYSQL* mysql)
{
	mysql_init(mysql);
	mysql_options(mysql, MYSQL_READ_DEFAULT_GROUP, "mydb");
	if (!mysql_real_connect(mysql, HOSTNAME, USERNAME, PASSWORD,DATABASE, 0, NULL, 0)) 
	{
	   error("Could not connect to host",mysql);
	}
}


void freeString(void* data)
{
	free((char*)data);
}

/* Lib functions */
int updateStream(struct userPost *st)
{
	/*cleanup*/
	if (st->streamname[strlen(st->streamname)-1] == '\n')
		st->streamname[strlen(st->streamname)-1] = '\0';

	/* SQL */
	MYSQL mysql;
	/*MYSQL_RES *res;
	MYSQL_ROW row;
	MYSQL_FIELD *field;*/
	char query[MAX_QUERY];

	connectToDB(&mysql);
	if (checkUserExists(&mysql, st->username, st->streamname)==0)
	{
		printf("User does not have access\n");
		return 1;
	}

	if (tableExists(&mysql, "streamData") == 0)
	{
		createSteamTable(&mysql);
		printf("Table was created\n");
	}

	/* insert post here */
	*query = 0;
	sprintf(query, "insert into streamData values (null,'%s','%s', null, '%s')", st->streamname, st->username, st->text);
	if(mysql_query(&mysql, query))
		error("Could not insert post",&mysql);
	

	mysql_close(&mysql);
	/* end sql */
	printf("Success\n");
	return 0;
}

int addUser(char* username, char* list)
{

	/* SQL */
	MYSQL mysql;
	/*MYSQL_RES *res;
	MYSQL_ROW row;
	MYSQL_FIELD *field;*/
	char query[MAX_QUERY];

	connectToDB(&mysql);

	if (tableExists(&mysql, "userData") == 0)
	{
		createUserTable(&mysql);
		printf("Table was created\n");
	}
	/* end sql */

	char stream[256];
	int a = 0;
	stream[0] = '\0';
	while (1)
	{
		if (list[a] == ',' || list[a] == '\0')
		{
			/* add user to stream here */
			if (checkUserExists(&mysql, username, stream))
				printf("User already has access to %s\n", stream);
			else
			{
				*query = 0;
				sprintf(query, "insert into userData values (null,'%s','%s',0)", username, stream);
				if(mysql_query(&mysql, query))
					error("Could not insert record",&mysql);
			}
			
			
			if (list[a] == '\0')
				break;
		}
		else
			append(stream, list[a]);
		a++;
	}
	mysql_close(&mysql);
	printf("Success\n");
	return 0;
}

int removeUser(char* username, char* list)
{

	/* SQL */
	MYSQL mysql;
	/*MYSQL_RES *res;
	MYSQL_ROW row;
	MYSQL_FIELD *field;*/
	char query[MAX_QUERY];

	connectToDB(&mysql);
	/* end sql */

	char stream[256];
	int a = 0;
	stream[0] = '\0';
	while (1)
	{
		if (list[a] == ',' || list[a] == '\0')
		{
			/* remove user from stream here */
			*query = 0;
			sprintf(query, "Delete from userData where userID = '%s' and streamID = '%s'", username, stream);
			if(mysql_query(&mysql, query))
				error("Could not remove user from stream",&mysql);
			if (list[a] == '\0')
				break;
		}
		else
			append(stream, list[a]);
		a++;
	}

	mysql_close(&mysql);
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