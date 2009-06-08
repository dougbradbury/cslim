#include "SymbolTable.h"
#include <stdlib.h>
#include <memory.h>
#include "SlimUtil.h"

typedef struct symbolNode {
	struct symbolNode* next;
	char* name;
	char* value;
} SymbolNode;

struct SymbolTable
{
	SymbolNode * head;
};

SymbolTable* SymbolTable_Create(void)
{
     SymbolTable* self = (SymbolTable*)malloc(sizeof(SymbolTable));
     memset(self, 0, sizeof(SymbolTable));
     return self;
}

void SymbolTable_Destroy(SymbolTable* self)
{
	SymbolNode* node;
	for (node = self->head; node;) {
		SymbolNode* nextSymbolNode = node->next;
		free(node->name);
		free(node->value);
		free(node);
		node = nextSymbolNode;
	}	
	free(self);
}

char * SymbolTable_FindSymbol(SymbolTable* self, char * name, int length) {
	SymbolNode* node;
	for (node = self->head; node; node = node->next)
	{
		if (strlen(node->name) == length && strncmp(node->name, name, length) == 0)
			return node->value;
	}
	return NULL;
}

void SymbolTable_SetSymbol(SymbolTable* self, char* symbol, char* value) {
	SymbolNode * symbolNode = (SymbolNode * )malloc(sizeof(SymbolNode));
	symbolNode->name = buyString(symbol);
	symbolNode->value = buyString(value);
	symbolNode->next = self->head;
	self->head = symbolNode;
}

int SymbolTable_GetSymbolLength(SymbolTable* self, char* symbol, int length)
{
	char * symbolValue = SymbolTable_FindSymbol(self, symbol, length);
	if (symbolValue == NULL)
		return -1;
	return strlen(symbolValue);
}


