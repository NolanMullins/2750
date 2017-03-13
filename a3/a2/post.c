#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <time.h> 
#include "stream.h" /* post program */
struct PostEntry { 
    struct userPost post; 
    char stream [256]; 
    char textArr [64]  [256]; 
    int numLine; 
    void (*PostEntryreadInputc) ( char *name , struct PostEntry* myStruct999 ); 
    void (*PostEntrygetTimeDatec) ( char *timeString , struct PostEntry* myStruct999 ); 
    void (*PostEntryformatEntry) ( struct PostEntry* myStruct999 ); 
    int (*PostEntrysubmitPost) ( struct PostEntry* myStruct999 ); 
}; 
void PostEntryreadInputc ( char *name , struct PostEntry* myStruct999 ) { 
    myStruct999->post.username = name; 
    printf ( "stream: " ); 
    fgets ( myStruct999->stream , 255 , stdin ); 
    int a = 0; 
    while ( myStruct999->stream [a++] != '\0' ) if ( myStruct999->stream [a-1] == '\n' ) myStruct999->stream [a-1] = '\0'; 
    a = 0; 
    printf ( "text: " ); 
    while ( a <64 && fgets ( myStruct999->textArr [a++] , 255 , stdin ) != NULL ); 
    myStruct999->numLine = a - 1; 
    return; 
} 
void PostEntrygetTimeDatec ( char *timeString , struct PostEntry* myStruct999 ) { 
    time_t timeT; 
    time ( & timeT ); 
    struct tm *info = localtime ( & timeT ); 
    sprintf ( timeString , "%d/%d/%d:%d/%d/%d" , info ->tm_sec , info ->tm_min , info ->tm_hour , info ->tm_mday , info ->tm_mon + 1 , info ->tm_year + 1900 ); 
    /*sprintf(timeString, "%d/%d/%d", info->tm_mday, info->tm_mon+1, info->tm_year+1900);*/
} 
void PostEntryformatEntry ( struct PostEntry* myStruct999 ) { 
    /* put data into struct */
    char *curTime = malloc ( sizeof ( char ) *256 ); 
    myStruct999->PostEntrygetTimeDatec ( curTime ,  myStruct999 ); 
    myStruct999->post.date = curTime; 
    myStruct999->post.streamname = myStruct999->stream; 
    int size = myStruct999->numLine *256; 
    char *text = malloc ( sizeof ( char ) *size ); 
    strcpy ( text , myStruct999->textArr [0] ); 
    int a; 
    for ( a = 1; 
    a <myStruct999->numLine; 
    a ++ ) strcat ( text , myStruct999->textArr [a] ); 
    myStruct999->post.text = text; 
    return; 
} 
int PostEntrysubmitPost ( struct PostEntry* myStruct999 ) { 
    /* submit the post to the stream lib */
    /*printf("\nSubmitting post\n\n");
		printf("%s\n", post.username);
		printf("%s\n", post.streamname);
		printf("%s\n", post.date);
		printf("%s", post.text);*/
    int sts = updateStream ( & myStruct999->post ); 
    if ( sts <= - 1 ) { 
        printf ( "Error with stream: %d\n" , sts ); 
    } 
    free ( myStruct999->post.text ); 
    free ( myStruct999->post.date ); 
    return 0; 
} 
void constructorPostEntry ( struct PostEntry* myS ) { 
    myS->PostEntryreadInputc = PostEntryreadInputc; 
    myS->PostEntrygetTimeDatec = PostEntrygetTimeDatec; 
    myS->PostEntryformatEntry = PostEntryformatEntry; 
    myS->PostEntrysubmitPost = PostEntrysubmitPost; 
} 
int main ( int argc , char *argv [] ) { 
    if ( argc <2 ) printf ( "No username\n" ); 
    if ( argc <2 ) exit ( 0 ); 
    int a; 
    char name [256]; 
    strcpy ( name , argv [1] ); 
    for ( a = 2; 
    a <argc; 
    a ++ ) { 
        strcat ( name , " " ); 
        strcat ( name , argv [a] ); 
    } 
    if ( name [strlen(name)-1] == '\n' ) printf ( "Found newline\n" ); 
    /*name[strlen(name)-1] = '\0';*/
    struct PostEntry entry; 
    constructorPostEntry(&entry); 
    entry.PostEntryreadInputc ( name , &entry ); 
    /*char curTime[256];
	entry.getTimeDate(curTime);*/
    entry.PostEntryformatEntry ( &entry ); 
    entry.PostEntrysubmitPost ( &entry ); 
    return 0; 
} 
