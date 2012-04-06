#include "stdafx.h"

#include <SlimList.h>
#include <StatementExecutor.h>

#include <string>

using namespace Slim;

BOOST_TEST_DONT_PRINT_LOG_VALUE(SlimList);

extern void AddFixtures(StatementExecutor*);

struct InitStatementExecutorTest
{
  InitStatementExecutorTest()
  {
    AddFixtures(&statementExecutor);
    statementExecutor.Make("test_slim", "TestSlim", &empty);
  }

  ~InitStatementExecutorTest()
  {
  }

  StatementExecutor statementExecutor;
  SlimList args;
  SlimList empty;
};

BOOST_FIXTURE_TEST_SUITE(StatementExecutorTestSuite, InitStatementExecutorTest)


BOOST_AUTO_TEST_CASE(CantCallFunctionThatDoesNotExist)
{
  std::string result = statementExecutor.Call("test_slim", "noSuchMethod", &args);
  BOOST_CHECK_EQUAL("__EXCEPTION__:message:<<NO_METHOD_IN_CLASS noSuchMethod[0] TestSlim.>>", result);

  result = statementExecutor.Call("test_slim", "noOtherSuchMethod", &args);
  BOOST_CHECK_EQUAL("__EXCEPTION__:message:<<NO_METHOD_IN_CLASS noOtherSuchMethod[0] TestSlim.>>", result);
}

BOOST_AUTO_TEST_CASE(ShouldTruncateReallyLongNamedFunctionThatDoesNotExistTo32)
{
  std::string result = statementExecutor.Call("test_slim", "noOtherSuchMethod123456789022345678903234567890423456789052345678906234567890", &args);
  BOOST_CHECK(result.size() < 120);
  BOOST_CHECK_EQUAL("__EXCEPTION__:message:<<NO_METHOD_IN_CLASS noOtherSuchMethod123456789022345[0] TestSlim.>>", result);	
}

BOOST_AUTO_TEST_CASE(ShouldKnowNumberofArgumentsforNonExistantFunction)
{
  args.AddString("BlahBlah");
  std::string result = statementExecutor.Call("test_slim", "noSuchMethod", &args);
  BOOST_CHECK_EQUAL("__EXCEPTION__:message:<<NO_METHOD_IN_CLASS noSuchMethod[1] TestSlim.>>", result);
}

BOOST_AUTO_TEST_CASE(ShouldNotAllowACallToaNonexistentInstance)
{
  std::string result = statementExecutor.Call("noSuchInstance", "noArgs", &args);
  BOOST_CHECK_EQUAL("__EXCEPTION__:message:<<NO_INSTANCE noSuchInstance.>>", result);	
}

BOOST_AUTO_TEST_CASE(ShouldNotAllowAMakeOnANonexistentClass)
{
  std::string result = statementExecutor.Make("instanceName", "NoSuchClass", &empty);
  BOOST_CHECK_EQUAL("__EXCEPTION__:message:<<NO_CLASS NoSuchClass.>>", result);	
}

BOOST_AUTO_TEST_CASE(CanCallaMethodThatReturnsAValue) 
{
  std::string result = statementExecutor.Call("test_slim", "returnValue", &args);
  BOOST_CHECK_EQUAL("value", result);
}

BOOST_AUTO_TEST_CASE(CanCallaMethodThatTakesASlimList)
{
  args.AddString("hello world");
  std::string result = statementExecutor.Call("test_slim", "echo", &args);
  BOOST_CHECK_EQUAL("hello world", result);
}

BOOST_AUTO_TEST_CASE(CanCallTwoInstancesOfTheSameFixture)
{
  args.AddString("one");
  SlimList* args2 = new SlimList();
  args2->AddString("two");

  statementExecutor.Make("test_slim2", "TestSlim", &empty);
  statementExecutor.Call("test_slim", "setArg", &args);
  statementExecutor.Call("test_slim2", "setArg", args2);
  std::string one = statementExecutor.Call("test_slim", "getArg", &empty);
  std::string two = statementExecutor.Call("test_slim2", "getArg", &empty);
  BOOST_CHECK_EQUAL("one", one);
  BOOST_CHECK_EQUAL("two", two);
  delete args2;
}

BOOST_AUTO_TEST_CASE(CanCreateTwoDifferentFixtures)
{
  args.AddString("one");
  SlimList* args2 = new SlimList();
  args2->AddString("two");

  statementExecutor.Make("test_slim2", "TestSlimAgain", &empty);	
  statementExecutor.Call("test_slim", "setArg", &args);	
  statementExecutor.Call("test_slim2", "setArgAgain", args2);
  std::string one = statementExecutor.Call("test_slim", "getArg", &empty);
  std::string two = statementExecutor.Call("test_slim2", "getArgAgain", &empty);
  BOOST_CHECK_EQUAL("one", one);
  BOOST_CHECK_EQUAL("two", two);
  delete args2;
}

BOOST_AUTO_TEST_CASE(CanReplaceSymbolsInSubLists) 
{
  statementExecutor.SetSymbol("v2", "doug");
  SlimList* subList = new SlimList();
  subList->AddString("Hi $v2.");
  args.AddList(subList);

  std::string result = statementExecutor.Call("test_slim", "echo", &args);
  BOOST_CHECK(!result.empty());
  SlimList* returnedList = SlimList::Deserialize(result);
  BOOST_CHECK(returnedList);
  BOOST_CHECK_EQUAL(1, returnedList->GetLength());
  std::string element = returnedList->GetStringAt(0);
  BOOST_CHECK_EQUAL("Hi doug.", element);
  delete subList;	
  delete returnedList;
}

BOOST_AUTO_TEST_CASE(CanReplaceSymbolsInSubSubLists) 
{
  statementExecutor.SetSymbol("v2", "doug");
  SlimList* subList = new SlimList();
  SlimList* subSubList = new SlimList();
  subSubList->AddString("Hi $v2.");
  subList->AddList(subSubList);
  args.AddList(subList);

  std::string result = statementExecutor.Call("test_slim", "echo", &args);
  BOOST_CHECK(!result.empty());
  SlimList* returnedSubList = SlimList::Deserialize(result);
  BOOST_CHECK(returnedSubList);
  BOOST_CHECK_EQUAL(1, returnedSubList->GetLength());
  SlimList* returnedSubSubList = returnedSubList->GetListAt(0);
  BOOST_CHECK(returnedSubSubList);
  BOOST_CHECK_EQUAL(1, returnedSubSubList->GetLength());	
  std::string element = returnedSubSubList->GetStringAt(0);
  BOOST_CHECK(!element.empty());
  BOOST_CHECK_EQUAL("Hi doug.", element);

  delete subSubList;
  delete subList;	
  delete returnedSubList;
}

BOOST_AUTO_TEST_CASE(CanCreateFixtureWithArguments) 
{
  SlimList* constructionArgs = new SlimList();
  constructionArgs->AddString("hi");
  statementExecutor.Make("test_slim", "TestSlim", constructionArgs);
  std::string result = statementExecutor.Call("test_slim", "getConstructionArg", &empty);
  BOOST_CHECK_EQUAL("hi", result);

  delete constructionArgs;
}

BOOST_AUTO_TEST_CASE(CanCreateFixtureWithArgumentsThatHaveSymbols) 
{
  statementExecutor.SetSymbol("name", "doug");
  SlimList* constructionArgs = new SlimList();
  constructionArgs->AddString("hi $name");
  statementExecutor.Make("test_slim", "TestSlim", constructionArgs);
  std::string result = statementExecutor.Call("test_slim", "getConstructionArg", &empty);
  BOOST_CHECK_EQUAL("hi doug", result);

  delete constructionArgs;
}

BOOST_AUTO_TEST_CASE(CanCreateFixtureWithArgumentsThatHaveMultipleSymbols) 
{
  statementExecutor.SetSymbol("fname", "doug");
  statementExecutor.SetSymbol("lname", "bradbury");

  SlimList* constructionArgs = new SlimList();
  constructionArgs->AddString("hi $fname $lname");
  statementExecutor.Make("test_slim", "TestSlim", constructionArgs);
  std::string result = statementExecutor.Call("test_slim", "getConstructionArg", &empty);
  BOOST_CHECK_EQUAL("hi doug bradbury", result);

  delete constructionArgs;
}

BOOST_AUTO_TEST_CASE(FixtureConstructionFailsWithUserErrorMessage) 
{
  SlimList* constructionArgs = new SlimList();
  constructionArgs->AddString("hi doug");
  constructionArgs->AddString("ho doug");

  std::string result = statementExecutor.Make("test_slim", "TestSlim", constructionArgs);
  BOOST_CHECK_EQUAL("__EXCEPTION__:message:<<COULD_NOT_INVOKE_CONSTRUCTOR TestSlim xxx.>>", result);

  delete constructionArgs;
}

BOOST_AUTO_TEST_CASE(FixtureCanReturnError) 
{
  std::string result = statementExecutor.Call("test_slim", "returnError", &args);
  BOOST_CHECK_EQUAL("__EXCEPTION__:message:<<my exception.>>", result);	
}

BOOST_AUTO_TEST_CASE(CanHaveNullResult)
{
  std::string result = statementExecutor.Call("test_slim", "null", &args);
  BOOST_CHECK(result.empty());  
}

#if 0
BOOST_AUTO_TEST_CASE(CanCallFixtureDeclaredBackwards) 
{
  statementExecutor.Make("backwardsTestSlim", "TestSlimDeclaredLate", &empty);
  SlimList_AddString(args, "hi doug");
  char* result = statementExecutor.Call("backwardsTestSlim", "echo", args);
  BOOST_CHECK_EQUAL("hi doug", result);
}

BOOST_AUTO_TEST_CASE(StatementExecutor, canCallFixtureNotDeclared) 
{
  statementExecutor.Make("undeclaredTestSlim", "TestSlimUndeclared", empty);
  SlimList_AddString(args, "hi doug");
  char* result = statementExecutor.Call("undeclaredTestSlim", "echo", args);
  BOOST_CHECK_EQUAL("hi doug", result);
}
#endif

BOOST_AUTO_TEST_SUITE_END()