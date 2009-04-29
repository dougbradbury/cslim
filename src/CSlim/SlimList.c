#include "SlimList.h"
#include "SlimUtil.h"
#include "SlimListDeserializer.h"
#include "SlimListSerializer.h"
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

//static local variables
typedef struct Node Node;

struct Node {
	Node* next;
	char* string;
	SlimList* list;
};

struct SlimList {
	int length;
	Node* head;	
	Node* tail;
};

static void insertNode(SlimList* self, Node* node);

SlimList* SlimList_create(void)
{
     SlimList* self = malloc(sizeof(SlimList));
     memset(self, 0, sizeof(SlimList));
     return self;
}

void SlimList_destroy(SlimList* self)
{
	Node * node;
	Node * next;
	for(node = self->head; node; node = next)
	{
		if (node->string) 
			free(node->string);
		
		if (node->list)
			SlimList_destroy(node->list);
			
		next = node->next;
		free(node);
	}
    free(self);
}

void SlimList_addBuffer(SlimList* self, char* buffer, int length) 
{
	Node* newNode = malloc(sizeof(Node));
	newNode->next = 0;
	newNode->list = 0;
	
	insertNode(self, newNode);

	newNode->string = buyBuf(buffer, length);
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

int SlimList_getLength(SlimList* self)
{
	return self->length;
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

Node * SlimList_getNodeAt(SlimList* self, int index)
{
	int i;
	Node* node = self->head;

	if (index >= self->length)
		return 0;

	for (i = 0; i < index; i++)
	{
		node = node->next;
	}
	return node;
}

SlimList * SlimList_getListAt(SlimList* self, int index)
{
	Node * node = SlimList_getNodeAt(self, index);
	if (node)
	{
		if (node->list == 0)
			node->list = SlimList_deserialize(node->string);	
	}
	return node->list;
}

char * SlimList_getStringAt(SlimList* self, int index)
{
	Node* node = SlimList_getNodeAt(self, index);
	if(node == 0)
		return 0;
	return node->string;
}

void SlimList_replaceAt(SlimList* self, int index, char * replacementString)
{
	Node* node = SlimList_getNodeAt(self, index);
	if(node->list != 0){
		SlimList_destroy(node->list);
		node->list = 0;
	}
	char * newString = buyString(replacementString);
	free(node->string);
	node->string = newString;
}

static void insertNode(SlimList* self, Node* node)
{
	if (self->length == 0)
	{
		self->head = node;
	}
	else
	{
		self->tail->next = node;
	}
	self->tail = node;	
	self->length++;
}
