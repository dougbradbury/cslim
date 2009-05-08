#include "CppUTest/TestHarness.h"
#include <stdio.h>

extern "C"
{
#include "SymbolTable.h"
}

TEST_GROUP(SymbolTable)
{
    SymbolTable* symbolTable;

    void setup()
    {
      symbolTable = SymbolTable_Create();
    }
    
    void teardown()
    {
       SymbolTable_Destroy(symbolTable);
    }
};

TEST(SymbolTable, findNonExistentSymbolShouldReturnNull)
{
	POINTERS_EQUAL(NULL, SymbolTable_FindSymbol(symbolTable, "Hey", 3));
}

TEST(SymbolTable, findSymbolShouldReturnSymbol)
{
	SymbolTable_SetSymbol(symbolTable, "Hey", "You");
	STRCMP_EQUAL("You", SymbolTable_FindSymbol(symbolTable, "Hey", 3));
}

TEST(SymbolTable, CanGetLengthOfSymbol)
{
	SymbolTable_SetSymbol(symbolTable, "Hey", "1234567890");
	LONGS_EQUAL(10, SymbolTable_GetSymbolLength(symbolTable, "Hey", 3));
}

TEST(SymbolTable, CanGetLengthOfNonExistentSymbol)
{
	LONGS_EQUAL(-1, SymbolTable_GetSymbolLength(symbolTable, "Hey", 3));
}

