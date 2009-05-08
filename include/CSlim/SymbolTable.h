#ifndef D_SymbolTable_H
#define D_SymbolTable_H

///////////////////////////////////////////////////////////////////////////////
//
//  SymbolTable is responsible for ...
//
///////////////////////////////////////////////////////////////////////////////

typedef struct SymbolTable SymbolTable;

SymbolTable* SymbolTable_Create(void);
void SymbolTable_Destroy(SymbolTable*);
char * SymbolTable_FindSymbol(SymbolTable* self, char * name, int length);
void SymbolTable_SetSymbol(SymbolTable* self, char* symbol, char* value);
int SymbolTable_GetSymbolLength(SymbolTable* self, char* symbol, int length);
#endif  // D_SymbolTable_H
