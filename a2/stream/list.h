/* Nolan Mullins
 * 
 * Basic linked list with sorting
 * 
 * Call init() before using a List
 * listRemove will return a reference to the data stored at that node
 */

#ifndef __MULLINSN_LIST__
#define __MULLINSN_LIST__

#include <stdlib.h>

typedef struct node
{
	struct node* next;
	void* data;
} Node;

typedef struct head
{
	Node* list;
} List;

/* will print the msg and exi */
void throwError(char* msg);
/* will init a List* */
List* init();
/*will add the data to the back of the list*/
void listAdd (List* head, void* data);
/*will get the data at index */
void* listGet(List* head, int index);
/*will set index to data*/
void listSet(List* head, int index, void* data);
/*will insert data into index */
void listInsert(List* head, void* data, int index);
/* will return the length of the list*/
int listSize(List* head);
/* will remove the node at index and return the data in that node */
void* listRemove(List* head, int index);
/* will clear a list and free its data */
List* listClear(List* head, void (*des)(void* a));
/* will del the head of a list **call once empty */
void delHead(List* head);
/* will soirt the list based on the comparison function */
void sort(List* head, int (*cmp)(void* a, void* b));
void sort2(List* head, int (*cmp)(void* a, void* b));

/*//del
//Node* get(List* head, int index);*/
Node* createNode(void* data);


#endif