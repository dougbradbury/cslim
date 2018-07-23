#include "SymbolTable.h"
#include <stdlib.h>
#include <string.h>
#include "SlimUtil.h"

typedef struct symbolNode {
	struct symbolNode* next;
	const char* name;
	const char* value;
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
		free((void*)node->name);
		free((void*)node->value);
		free(node);
		node = nextSymbolNode;
	}	
	free(self);
}

const char * SymbolTable_FindSymbol(SymbolTable* self, char const * name, int length) {
	SymbolNode* node;
	for (node = self->head; node; node = node->next)
	{
		if (strlen(node->name) == length && strncmp(node->name, name, length) == 0)
			return node->value;
	}
	return NULL;
}

void SymbolTable_SetSymbol(SymbolTable* self, char const * symbol, char const * value) {
	SymbolNode * symbolNode = (SymbolNode * )malloc(sizeof(SymbolNode));
	symbolNode->name = CSlim_BuyString(symbol);
	symbolNode->value = CSlim_BuyString(value);
	symbolNode->next = self->head;
	self->head = symbolNode;
}

int SymbolTable_GetSymbolLength(SymbolTable* self, char const* symbol, int length)
{
	const char * symbolValue = SymbolTable_FindSymbol(self, symbol, length);
	if (symbolValue == NULL)
		return -1;
	return strlen(symbolValue);
}


