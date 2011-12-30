#ifndef D_SymbolTable_H
#define D_SymbolTable_H

typedef struct SymbolTable SymbolTable;

SymbolTable* SymbolTable_Create(void);
void SymbolTable_Destroy(SymbolTable*);
char * SymbolTable_FindSymbol(SymbolTable* self, char const* name, int length);
void SymbolTable_SetSymbol(SymbolTable* self, char const* symbol, char const* value);
int SymbolTable_GetSymbolLength(SymbolTable* self, char const* symbol, int length);
#endif
