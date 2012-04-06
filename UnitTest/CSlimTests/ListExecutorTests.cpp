#include "stdafx.h"

#include <ListExecutor.h>
#include <SlimList.h>
#include <StatementExecutor.h>

using namespace Slim;

BOOST_TEST_DONT_PRINT_LOG_VALUE(SlimList);

extern void AddFixtures(StatementExecutor*);

struct InitListExecutorTest
{
  InitListExecutorTest()
    : listExecutor(&statementExecutor)
  {
    AddFixtures(&statementExecutor);
    char const* import[] = {"i1", "import", "blah", 0};
    AddStatementTo(&instructions, import);
    char const * make[] = {"m1", "make", "test_slim", "TestSlim", 0};
    AddStatementTo(&instructions, make);
  }

  ~InitListExecutorTest()
  {
  }

  void AddStatementTo(SlimList* instructions, char const** elements)
  {
    SlimList* statement = new SlimList();
    while (*elements) 
    {
      statement->AddString(*elements++);
    }
    instructions->AddList(statement);
    delete statement;
  }

  ListExecutor listExecutor;
  SlimList instructions;
  StatementExecutor statementExecutor;
};

BOOST_FIXTURE_TEST_SUITE(ListExecutorTestSuite, InitListExecutorTest)


BOOST_AUTO_TEST_CASE(ImportShouldReturnOk)
{
  SlimList* results = listExecutor.Execute(&instructions);
  SlimList* importResult = results->GetListAt(0);
  BOOST_CHECK_EQUAL("i1", importResult->GetStringAt(0));
  BOOST_CHECK_EQUAL("OK", importResult->GetStringAt(1));
  delete results;
}

BOOST_AUTO_TEST_CASE(CannotExecuteAnInvalidOperation)
{
  char const* invalid[] = {"inv1", "Invalid", 0};
  AddStatementTo(&instructions, invalid);

  SlimList* results = listExecutor.Execute(&instructions);
  BOOST_CHECK_EQUAL(3, results->GetLength());

  SlimList* invalidStatementResult = results->GetListAt(2);
  BOOST_CHECK_EQUAL("inv1", invalidStatementResult->GetStringAt(0));
  BOOST_CHECK_EQUAL("__EXCEPTION__:message:<<INVALID_STATEMENT [\"inv1\", \"Invalid\"].>>", invalidStatementResult->GetStringAt(1));

  delete results;  
}

BOOST_AUTO_TEST_CASE(CanCallASimpleFunction)
{
  char const* call[] = {"call1", "call", "test_slim", "returnValue", 0};
  AddStatementTo(&instructions, call);

  SlimList* results = listExecutor.Execute(&instructions);
  BOOST_CHECK_EQUAL(3, results->GetLength());

  SlimList* makeResult = results->GetListAt(1);
  BOOST_CHECK_EQUAL("m1", makeResult->GetStringAt(0));
  BOOST_CHECK_EQUAL("OK", makeResult->GetStringAt(1));

  SlimList* callResult = results->GetListAt(2);
  BOOST_CHECK_EQUAL("call1", callResult->GetStringAt(0));
  BOOST_CHECK_EQUAL("value", callResult->GetStringAt(1));

  delete results;  
}

BOOST_AUTO_TEST_CASE(CantExecuteMalformedInstruction)
{
  char const* call[] = {"call1", "call", "notEnoughArguments", 0};
  AddStatementTo(&instructions, call);  

  SlimList* results = listExecutor.Execute(&instructions);
  SlimList* invalidStatementResult = results->GetListAt(2); 
  BOOST_CHECK_EQUAL("__EXCEPTION__:message:<<MALFORMED_INSTRUCTION [\"call1\", \"call\", \"notEnoughArguments\"].>>", invalidStatementResult->GetStringAt(1));

  delete results;  
}

BOOST_AUTO_TEST_CASE(CantCallAmethodOnAnInstanceThatDoesntExist)
{
  char const* call[] = {"call1", "call", "noSuchInstance", "method", 0};
  AddStatementTo(&instructions, call);  

  SlimList* results = listExecutor.Execute(&instructions);
  SlimList* invalidStatementResult = results->GetListAt(2); 
  BOOST_CHECK_EQUAL("__EXCEPTION__:message:<<NO_INSTANCE noSuchInstance.>>", invalidStatementResult->GetStringAt(1));

  delete results;  
}

BOOST_AUTO_TEST_CASE(ShouldRespondToAnEmptySetOfInstructionsWithAnEmptySetOfResults) 
{
  SlimList* instructions = new SlimList();
  SlimList* results = listExecutor.Execute(instructions);
  BOOST_CHECK_EQUAL(0, results->GetLength());
  delete results;
  delete instructions;
}

BOOST_AUTO_TEST_CASE(CanPassArgumentsToConstructor) 
{ 
  char const* make2[] = {"make2", "make", "test_slim2", "TestSlim", "ConstructorArgument", 0};
  char const* call[] = {"call1", "call", "test_slim2", "getConstructionArg", 0};
  AddStatementTo(&instructions, make2);
  AddStatementTo(&instructions, call);  

  SlimList* results = listExecutor.Execute(&instructions);
  SlimList* callResult = results->GetListAt(3); 
  BOOST_CHECK_EQUAL("ConstructorArgument", callResult->GetStringAt(1));

  delete results;
}

BOOST_AUTO_TEST_CASE(CanCallAFunctionMoreThanOnce)
{
  char const* call[] = {"call1", "call", "test_slim", "echo", "Hello", 0};
  AddStatementTo(&instructions, call);  
  char const* call2[] = {"call2", "call", "test_slim", "echo", "Goodbye", 0};
  AddStatementTo(&instructions, call2);  

  SlimList* results = listExecutor.Execute(&instructions);
  SlimList* callResult = results->GetListAt(2); 
  BOOST_CHECK_EQUAL("Hello", callResult->GetStringAt(1));

  callResult = results->GetListAt(3); 
  BOOST_CHECK_EQUAL("Goodbye", callResult->GetStringAt(1));

  delete results;
}

BOOST_AUTO_TEST_CASE(CanAssignTheReturnValueToASymbol)
{
  char const* call[] = {"id1", "callAndAssign", "v", "test_slim", "add", "x", "y", 0};
  AddStatementTo(&instructions, call);  

  char const* call2[] = {"id2", "call", "test_slim", "echo", "$v", 0};
  AddStatementTo(&instructions, call2);  

  SlimList* results = listExecutor.Execute(&instructions);
  SlimList* callResult = results->GetListAt(2); 
  BOOST_CHECK_EQUAL("xy", callResult->GetStringAt(1));

  callResult = results->GetListAt(3); 
  BOOST_CHECK_EQUAL("xy", callResult->GetStringAt(1));

  delete results;
}

BOOST_AUTO_TEST_CASE(CanReplateMultipleSymbolsInASingleArgument)
{
  char const* call[] = {"id1", "callAndAssign", "v1", "test_slim", "echo", "Bob", 0};
  AddStatementTo(&instructions, call);  

  char const* call2[] = {"id2", "callAndAssign", "v2", "test_slim", "echo", "Martin", 0};
  AddStatementTo(&instructions, call2);  

  char const* call3[] = {"id2", "call", "test_slim", "echo", "name:  $v1 $v2 $12.23", 0};
  AddStatementTo(&instructions, call3);  

  SlimList* results = listExecutor.Execute(&instructions);
  SlimList* callResult = results->GetListAt(4); 
  BOOST_CHECK_EQUAL("name:  Bob Martin $12.23", callResult->GetStringAt(1));

  delete results;
}

BOOST_AUTO_TEST_CASE(CanPassAndReturnAList)
{
  SlimList* l = new SlimList();
  l->AddString("1");
  l->AddString("2");

  SlimList* statement = new SlimList();
  statement->AddString("id1");
  statement->AddString("call");
  statement->AddString("test_slim");
  statement->AddString("echo");
  statement->AddList(l);

  instructions.AddList(statement);
  delete statement;

  SlimList* results = listExecutor.Execute(&instructions);
  SlimList* callResult = results->GetListAt(2); 
  SlimList* resultList = callResult->GetListAt(1);
  BOOST_CHECK_EQUAL(*l, *resultList);

  delete results;
  delete l;  
}

BOOST_AUTO_TEST_CASE(CanReturnNull)
{
  char const* call[] = {"id1", "call", "test_slim", "null", 0};
  AddStatementTo(&instructions, call);

  SlimList* results = listExecutor.Execute(&instructions);
  SlimList* callResult = results->GetListAt(2); 
  BOOST_CHECK_EQUAL("null", callResult->GetStringAt(1));

  delete results;  
}

BOOST_AUTO_TEST_CASE(CanPassASymbolInAList)
{
  char const* call[] = {"id1", "callAndAssign", "v", "test_slim", "echo", "Bob", 0};
  AddStatementTo(&instructions, call);  

  SlimList* statement = new SlimList();
  statement->AddString("id2");
  statement->AddString("call");
  statement->AddString("test_slim");
  statement->AddString("echo");

  SlimList* l = new SlimList();
  l->AddString("$v");
  statement->AddList(l);

  instructions.AddList(statement);
  delete statement;

  SlimList* results = listExecutor.Execute(&instructions);
  SlimList* callResult = results->GetListAt(3); 
  SlimList* resultList = callResult->GetListAt(1);

  SlimList* expected = new SlimList();
  expected->AddString("Bob");

  BOOST_CHECK_EQUAL(*expected, *resultList);

  delete results;
  delete l;  
  delete expected;
}

BOOST_AUTO_TEST_SUITE_END()
