#ifndef __MULLINSN_STREAM__
#define __MULLINSN_STREAM__

struct userPost { 
    char *username; 
    char *streamname; 
    char *date; 
    char *text; 
}; 

int userExists(char* name, char* file);
int updateStream(struct userPost *st);
int addUser(char* username, char* list);
int removeUser(char* username, char* list);

#endif