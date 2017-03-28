#ifndef __MULLINSN_DATA__
#define __MULLINSN_DATA__

#include "list.h"

typedef struct element
{
	char tag;
	List* data;
} Element;

#endif