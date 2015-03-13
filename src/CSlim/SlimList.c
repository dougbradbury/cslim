#include "SlimList.h"
#include "SlimUtil.h"
#include "SlimListDeserializer.h"
#include "SlimListSerializer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

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
static void SlimList_DestroyNode(Node* node);

SlimList* SlimList_Create(void)
{
	 SlimList* self = (SlimList*)malloc(sizeof(SlimList));
	 memset(self, 0, sizeof(SlimList));
	 return self;
}

void SlimList_Destroy(SlimList* self)
{
	Node * node;
	Node * next;

	for(node = self->head; node; node = next)
	{
		next = node->next;
		SlimList_DestroyNode(node);
	}
	free(self);
}

static void SlimList_DestroyNode(Node* node) {
	if (node->string)
		free(node->string);

	if (node->list)
		SlimList_Destroy(node->list);

	free(node);
}

SlimListIterator* SlimList_CreateIterator(SlimList* list) {
	return list->head;
}

int SlimList_Iterator_HasItem(SlimListIterator* iterator) {
	return iterator != NULL;
}

void SlimList_Iterator_Advance(SlimListIterator** iterator) {
	if (*iterator != NULL)
		*iterator = (*iterator)->next;
}

void SlimList_AddBuffer(SlimList* self, char const* buffer, int length)
{
	Node* newNode = (Node*)malloc(sizeof(Node));
	newNode->next = 0;
	newNode->list = 0;

	insertNode(self, newNode);

	newNode->string = CSlim_BuyBuf(buffer, length);
}


void SlimList_AddString(SlimList* self, char const* string)
{
	SlimList_AddBuffer(self, string, string ? (int)strlen(string) : 0);
}

void SlimList_AddList(SlimList* self, SlimList* element)
{
	char * embedded = SlimList_Serialize(element);
	SlimList_AddString(self, embedded);
	SlimList_Release(embedded);
}

void SlimList_PopHead(SlimList* self)
{
	assert(self->head != NULL);

	Node* previousHead = self->head;
	self->head = previousHead->next;

	if (self->tail == previousHead)
		self->tail = NULL;

	self->length--;

	SlimList_DestroyNode(previousHead);
}

int SlimList_GetLength(SlimList* self)
{
	return self->length;
}

int SlimList_Equals(SlimList* self, SlimList* other){
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

Node* SlimList_GetNodeAt(SlimList* self, int index)
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

SlimList * SlimList_GetListAt(SlimList* self, int index)
{
	Node * node = SlimList_GetNodeAt(self, index);
	return SlimList_Iterator_GetList(node);
}

SlimList* SlimList_Iterator_GetList(SlimListIterator* iterator)
{
	assert(iterator != NULL);

	if (iterator->list == 0)
		iterator->list = SlimList_Deserialize(iterator->string);

	return iterator->list;
}

char* SlimList_GetStringAt(SlimList* self, int index)
{
	Node* node = SlimList_GetNodeAt(self, index);
	if(node == 0)
		return 0;

	return SlimList_Iterator_GetString(node);
}

char* SlimList_Iterator_GetString(SlimListIterator* iterator)
{
	assert(iterator != NULL);
	return iterator->string;
}

double SlimList_GetDoubleAt(SlimList* self, int index)
{
  char* speed_s = SlimList_GetStringAt(self, index);
  return atof(speed_s);
}

static char * parseHashCell(char ** cellStart)
{
	char * cellValue = *cellStart + strlen("<td>");
	char * cellStop = strstr(cellValue, "</td>");

	int length = (int)(cellStop - cellValue);
	char * buf = (char*)malloc(length + 1);
	strncpy(buf, cellValue, length);
	buf[length] = 0;
	
	*cellStart = strstr(cellStop + strlen("<td>"), "<td>");
	
	return buf;
}

static SlimList* parseHashEntry(char * row)
{
		SlimList * element = SlimList_Create();

		char * cellStart = strstr(row, "<td>");

		char* hashKey = parseHashCell(&cellStart);
		SlimList_AddString(element, hashKey);
		free(hashKey);

		char * hashValue = parseHashCell(&cellStart);
		SlimList_AddString(element, hashValue);
		free(hashValue);
		
		return element;
}

static SlimList* SlimList_deserializeHash(char * serializedHash)
{	
  SlimList *element;
  SlimList *hash = SlimList_Create();
  
  char * row = strstr(serializedHash, "<tr>");
  while (row != NULL)
	{
		element = parseHashEntry(row);
		SlimList_AddList(hash, element);
		SlimList_Destroy(element);
		row = strstr(row + strlen("<tr>"), "<tr>");
	}
  return hash;	
}

SlimList* SlimList_GetHashAt(SlimList* self, int index)
{
	return SlimList_deserializeHash(SlimList_GetStringAt(self, 0));
}

void SlimList_ReplaceAt(SlimList* self, int index, char const * replacementString)
{
	Node* node = SlimList_GetNodeAt(self, index);
	SlimList_Iterator_Replace(node, replacementString);
}

void SlimList_Iterator_Replace(SlimListIterator* iterator, const char* replacementString) {
	if (iterator->list != 0) {
		SlimList_Destroy(iterator->list);
		iterator->list = 0;
	}
	char* newString = CSlim_BuyString(replacementString);
	free(iterator->string);
	iterator->string = newString;
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

void SlimList_Iterator_AdvanceBy(SlimListIterator** iterator, int amount) {
	int i;
	for (i = 0; i < amount; i++) {
		SlimList_Iterator_Advance(iterator);
	}
}

SlimList* SlimList_GetTailAt(SlimList* self, int index)
{
	SlimList * tail = SlimList_Create();

	SlimListIterator* iterator = SlimList_CreateIterator(self);
	SlimList_Iterator_AdvanceBy(&iterator, index);

	while (SlimList_Iterator_HasItem(iterator))
	{
		SlimList_AddString(tail, SlimList_Iterator_GetString(iterator));
		SlimList_Iterator_Advance(&iterator);
	}

	return tail;
}

char* SlimList_ToString(SlimList* self) {
	char* result = CSlim_CreateEmptyString();
	CSlim_ConcatenateString(&result, "[");

	SlimListIterator* iterator = SlimList_CreateIterator(self);
	while (SlimList_Iterator_HasItem(iterator)) {
		SlimList* sublist = SlimList_Iterator_GetList(iterator);

		if (sublist != NULL) {
			char* subListAsAString = SlimList_ToString(sublist);
			CSlim_ConcatenateString(&result, subListAsAString);
			CSlim_DestroyString(subListAsAString);
		} else {
			CSlim_ConcatenateString(&result, "\"");
			CSlim_ConcatenateString(&result, SlimList_Iterator_GetString(iterator));
			CSlim_ConcatenateString(&result, "\"");
		}

		if (iterator->next != NULL) {
			CSlim_ConcatenateString(&result, ", ");
		}
		SlimList_Iterator_Advance(&iterator);
	}
	CSlim_ConcatenateString(&result, "]");

	return result;
}
