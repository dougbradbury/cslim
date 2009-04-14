#include "SlimList.h"
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

enum {LIST_OVERHEAD=9, ELEMENT_OVERHEAD=8};

//static local variables
typedef struct Node Node;

struct Node {
	Node* next;
	char* string;
};

struct SlimList {
	int length;
	Node* head;	
	Node* tail;
};

SlimList* SlimList_Create(void)
{
     SlimList* self = malloc(sizeof(SlimList));
     memset(self, 0, sizeof(SlimList));
     return self;
}

void SlimList_Destroy(SlimList* self)
{
	Node * node;
	Node * next;
	for(node = self->head; node; node = next)
	{
		if (node->string) 
			free(node->string);
		next = node->next;
		free(node);
	}
    free(self);
}


char* SlimList_serialize(SlimList* self)
{
	char* buf = malloc(SlimList_serializedLength(self)+1);
	char* write_ptr = buf;
	Node * node;
	write_ptr += sprintf(write_ptr, "[%06d:", self->length);
	for(node = self->head; node; node = node->next)
	{
		write_ptr += sprintf(write_ptr, "%06ld:%s:", strlen(node->string), node->string);
	}
	strcpy(write_ptr, "]");
	return buf;
}

void SlimList_addBuffer(SlimList* self, char* buffer, int length) 
{
	Node* newNode = malloc(sizeof(Node));
	newNode->next = 0;
	
	if (self->length == 0)
	{
		self->head = newNode;
	}
	else
	{
		self->tail->next = newNode;
	}
	self->tail = newNode;	
	
	newNode->string = malloc(length+1);
	strncpy(newNode->string, buffer, length);
	newNode->string[length] = 0;
	self->length++;
}

void SlimList_addString(SlimList* self, char* string) 
{
	SlimList_addBuffer(self, string, strlen(string));
}

void SlimList_addList(SlimList* self, SlimList* element)
{
	char * embedded = SlimList_serialize(element);
	SlimList_addString(self, embedded);
	free(embedded);
}

int SlimList_serializedLength(SlimList* self)
{
	int length = LIST_OVERHEAD;
	Node* node;
	for (node = self->head; node; node=node->next) 
	{
		length += strlen(node->string) + ELEMENT_OVERHEAD;
	}
	return length;
}

int SlimList_getLength(SlimList* self)
{
	return self->length;
}

#define SKIP(a) \
if (*current != (a))\
{\
	SlimList_Destroy(list); \
	return 0;\
}\
current++;

int readLength(char** readPtr)
{
	int length = atoi(*readPtr);
	*readPtr += 6;	
	return length;
}

SlimList* SlimList_deserialize(char* serializedList)
{
	int listLength;
	SlimList * list = 0;
	char* current = 0;
	
	if(serializedList == 0 || strlen(serializedList) == 0)
		return 0;
	
	current = serializedList;
	list = SlimList_Create();

	SKIP('[')
	
	listLength = readLength(&current);
	
	SKIP(':')
	
	while (listLength--)
	{
		int elementLength = readLength(&current);
		SKIP(':')
		SlimList_addBuffer(list, current, elementLength);
		current += elementLength;
		SKIP(':')
	}

	SKIP(']')
	return list;
}

int SlimList_equals(SlimList* self, SlimList* other){
	Node *p, *q;
	if (self->length != other->length)
		return 0;

	for (p = self->head, q=other->head; p; p=p->next, q=q->next) 
	{
		if (strcmp(p->string, q->string) != 0)
			return 0;	
	}
	
	return 1;
}

