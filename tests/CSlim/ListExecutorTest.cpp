#include "CppUTest/TestHarness.h"


extern "C"
{
#include "ListExecutor.h"
#include "SlimList.h"
#include "StatementExecutor.h"
#include "TestSlim.h"
}

TEST_GROUP(ListExecutor)
{
    ListExecutor* listExecutor;
    SlimList* instructions;
    StatementExecutor * statementExecutor;
    void setup()
    {
      statementExecutor = StatementExecutor_Create();
      StatementExecutor_AddFixture(statementExecutor, TestSlim_Register);
      listExecutor = ListExecutor_Create(statementExecutor);
      instructions = SlimList_Create();
      char const * import[] = {"i1", "import", "blah", 0};
      addStatementTo(instructions, import);
      char const * make[] = {"m1", "make", "test_slim", "TestSlim", 0};
      addStatementTo(instructions, make);
      
    }
    
    void teardown()
    {
       ListExecutor_Destroy(listExecutor);
       SlimList_Destroy(instructions);
       StatementExecutor_Destroy(statementExecutor);
    }
    
    void addStatementTo(SlimList* instructions, char const** elements)
    {
      SlimList* statement = SlimList_Create();
      while (*elements) {
        SlimList_AddString(statement, *elements++);
      }
      SlimList_AddList(instructions, statement);
      SlimList_Destroy(statement);
    }
};

TEST(ListExecutor, ImportShouldReturnOk)
{
  SlimList * results = ListExecutor_Execute(listExecutor, instructions);
  SlimList * importResult = SlimList_GetListAt(results, 0);
  STRCMP_EQUAL("i1", SlimList_GetStringAt(importResult, 0));
  STRCMP_EQUAL("OK", SlimList_GetStringAt(importResult, 1));
  SlimList_Destroy(results);
}

TEST(ListExecutor, CannotExecuteAnInvalidOperation)
{
  char const * invalid[] = {"inv1", "Invalid", 0};
  addStatementTo(instructions, invalid);

  SlimList * results = ListExecutor_Execute(listExecutor, instructions);
  LONGS_EQUAL(3, SlimList_GetLength(results));
  SlimList * invalidStatementResult = SlimList_GetListAt(results, 2);
  
  
  STRCMP_EQUAL("inv1", SlimList_GetStringAt(invalidStatementResult, 0));
  STRCMP_EQUAL("__EXCEPTION__:message:<<INVALID_STATEMENT: [\"inv1\", \"Invalid\"].>>", SlimList_GetStringAt(invalidStatementResult, 1));
  SlimList_Destroy(results);  
}

TEST(ListExecutor, CanCallASimpleFunction)
{
  char const * call[] = {"call1", "call", "test_slim", "returnValue", 0};
  addStatementTo(instructions, call);


  SlimList * results = ListExecutor_Execute(listExecutor, instructions);
  LONGS_EQUAL(3, SlimList_GetLength(results));

  SlimList * makeResult = SlimList_GetListAt(results, 1);
  STRCMP_EQUAL("m1", SlimList_GetStringAt(makeResult, 0));
  STRCMP_EQUAL("OK", SlimList_GetStringAt(makeResult, 1));
  
  SlimList * callResult = SlimList_GetListAt(results, 2);
  STRCMP_EQUAL("call1", SlimList_GetStringAt(callResult, 0));
  STRCMP_EQUAL("value", SlimList_GetStringAt(callResult, 1));
  
  SlimList_Destroy(results);  
    
}

TEST(ListExecutor, CantExecuteMalformedInstruction)
{
  char const * call[] = {"call1", "call", "notEnoughArguments", 0};
  addStatementTo(instructions, call);  
  SlimList * results = ListExecutor_Execute(listExecutor, instructions);
  SlimList * invalidStatementResult = SlimList_GetListAt(results, 2); 
  STRCMP_EQUAL("__EXCEPTION__:message:<<MALFORMED_INSTRUCTION [\"call1\", \"call\", \"notEnoughArguments\"].>>", SlimList_GetStringAt(invalidStatementResult, 1));
  SlimList_Destroy(results);  
}

TEST(ListExecutor, CantCallAmethodOnAnInstanceThatDoesntExist)
{
  char const * call[] = {"call1", "call", "noSuchInstance", "method", 0};
  addStatementTo(instructions, call);  
  SlimList * results = ListExecutor_Execute(listExecutor, instructions);
  SlimList * invalidStatementResult = SlimList_GetListAt(results, 2); 
  STRCMP_EQUAL("__EXCEPTION__:message:<<NO_INSTANCE noSuchInstance.>>", SlimList_GetStringAt(invalidStatementResult, 1));
  SlimList_Destroy(results);
}

TEST(ListExecutor, ShouldRespondToAnEmptySetOfInstructionsWithAnEmptySetOfResults) 
{
  SlimList* instructions = SlimList_Create();
  SlimList * results = ListExecutor_Execute(listExecutor, instructions);
  LONGS_EQUAL(0, SlimList_GetLength(results));
  SlimList_Destroy(results);
  SlimList_Destroy(instructions);
}

TEST(ListExecutor, CanPassArgumentsToConstructor) 
{ 
  char const * make2[] = {"make2", "make", "test_slim2", "TestSlim", "ConstructorArgument", 0};
  char const * call[] = {"call1", "call", "test_slim2", "getConstructionArg", 0};
  addStatementTo(instructions, make2);
  addStatementTo(instructions, call);  
  SlimList * results = ListExecutor_Execute(listExecutor, instructions);
  SlimList * callResult = SlimList_GetListAt(results, 3); 
  STRCMP_EQUAL("ConstructorArgument", SlimList_GetStringAt(callResult, 1));
  SlimList_Destroy(results);
}

TEST(ListExecutor, CanCallAFunctionMoreThanOnce)
{
  char const * call[] = {"call1", "call", "test_slim", "echo", "Hello", 0};
  addStatementTo(instructions, call);  
  char const * call2[] = {"call2", "call", "test_slim", "echo", "Goodbye", 0};
  addStatementTo(instructions, call2);  
  
  SlimList * results = ListExecutor_Execute(listExecutor, instructions);

  SlimList * callResult = SlimList_GetListAt(results, 2); 
  STRCMP_EQUAL("Hello", SlimList_GetStringAt(callResult, 1));
  callResult = SlimList_GetListAt(results, 3); 
  STRCMP_EQUAL("Goodbye", SlimList_GetStringAt(callResult, 1));

  SlimList_Destroy(results);
}

TEST(ListExecutor, CanAssignTheReturnValueToASymbol)
{
  char const * call[] = {"id1", "callAndAssign", "v", "test_slim", "add", "x", "y", 0};
  addStatementTo(instructions, call);  

  char const * call2[] = {"id2", "call", "test_slim", "echo", "$v", 0};
  addStatementTo(instructions, call2);  

  SlimList * results = ListExecutor_Execute(listExecutor, instructions);

  SlimList * callResult = SlimList_GetListAt(results, 2); 
  STRCMP_EQUAL("xy", SlimList_GetStringAt(callResult, 1));
  callResult = SlimList_GetListAt(results, 3); 
  STRCMP_EQUAL("xy", SlimList_GetStringAt(callResult, 1));

  SlimList_Destroy(results);
}

TEST(ListExecutor, CanReplateMultipleSymbolsInASingleArgument)
{
  char const * call[] = {"id1", "callAndAssign", "v1", "test_slim", "echo", "Bob", 0};
  addStatementTo(instructions, call);  
  
  char const * call2[] = {"id2", "callAndAssign", "v2", "test_slim", "echo", "Martin", 0};
  addStatementTo(instructions, call2);  
  
  char const * call3[] = {"id2", "call", "test_slim", "echo", "name:  $v1 $v2 $12.23", 0};
  addStatementTo(instructions, call3);  
  
  SlimList * results = ListExecutor_Execute(listExecutor, instructions);

  SlimList * callResult = SlimList_GetListAt(results, 4); 
  STRCMP_EQUAL("name:  Bob Martin $12.23", SlimList_GetStringAt(callResult, 1));

  SlimList_Destroy(results);
}

TEST(ListExecutor, CanPassAndReturnAList)
{
  SlimList* l = SlimList_Create();
  SlimList_AddString(l, "1");
  SlimList_AddString(l, "2");
  
  SlimList* statement = SlimList_Create();
  SlimList_AddString(statement, "id1");
  SlimList_AddString(statement, "call");
  SlimList_AddString(statement, "test_slim");
  SlimList_AddString(statement, "echo");
  SlimList_AddList(statement, l);
    
  SlimList_AddList(instructions, statement);
  SlimList_Destroy(statement);
  
  SlimList * results = ListExecutor_Execute(listExecutor, instructions);

  SlimList * callResult = SlimList_GetListAt(results, 2); 
  SlimList * resultList = SlimList_GetListAt(callResult, 1);
  CHECK(SlimList_Equals(l, resultList));

  SlimList_Destroy(results);
  SlimList_Destroy(l);  
}

TEST(ListExecutor, CanReturnNull)
{
  char const * call[] = {"id1", "call", "test_slim", "null", 0};
  addStatementTo(instructions, call);
  
  SlimList * results = ListExecutor_Execute(listExecutor, instructions);

  SlimList * callResult = SlimList_GetListAt(results, 2); 
  
  STRCMP_EQUAL("null", SlimList_GetStringAt(callResult, 1));
  
  SlimList_Destroy(results);  
}

TEST(ListExecutor, CanPassASymbolInAList)
{
  char const * call[] = {"id1", "callAndAssign", "v", "test_slim", "echo", "Bob", 0};
  addStatementTo(instructions, call);  
    
  SlimList* statement = SlimList_Create();
  SlimList_AddString(statement, "id2");
  SlimList_AddString(statement, "call");
  SlimList_AddString(statement, "test_slim");
  SlimList_AddString(statement, "echo");

  SlimList* l = SlimList_Create();
  SlimList_AddString(l, "$v");
  SlimList_AddList(statement, l);
    
  SlimList_AddList(instructions, statement);
  SlimList_Destroy(statement);
  
  SlimList * results = ListExecutor_Execute(listExecutor, instructions);

  SlimList * callResult = SlimList_GetListAt(results, 3); 
  SlimList * resultList = SlimList_GetListAt(callResult, 1);

  SlimList* expected = SlimList_Create();
  SlimList_AddString(expected, "Bob");

  CHECK(SlimList_Equals(expected, resultList));

  SlimList_Destroy(results);
  SlimList_Destroy(l);  
  SlimList_Destroy(expected);
  
}
