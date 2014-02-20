#include "CppUTest/TestHarness.h"

extern "C"
{
#include "StatementExecutor.h"
#include "TestSlim.h"
#include "SlimList.h"
#include "SlimListDeserializer.h"
#include <string.h>
#include <stdio.h>
}

TEST_GROUP(StatementExecutor)
{
  StatementExecutor* statementExecutor;
	SlimList* args;
	SlimList* empty;

  void setup()
  {
  	args = SlimList_Create();
		empty = SlimList_Create();
		statementExecutor = StatementExecutor_Create();
		StatementExecutor_AddFixture(statementExecutor, TestSlim_Register);
		StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", empty);
    }
    
    void teardown()
    {
		StatementExecutor_Destroy(statementExecutor);
		SlimList_Destroy(args);
		SlimList_Destroy(empty);
    }
};

TEST(StatementExecutor, canCallFunctionWithNoArguments)
{
	StatementExecutor_Call(statementExecutor, "test_slim", "noArgs", args);
	TestSlim* testSlim = (TestSlim*)StatementExecutor_Instance(statementExecutor, "test_slim");
	CHECK(TestSlim_noArgsCalled(testSlim));
}

TEST(StatementExecutor, cantCallFunctionThatDoesNotExist)
{
	char* result = StatementExecutor_Call(statementExecutor, "test_slim", "noSuchMethod", args);
	STRCMP_EQUAL("__EXCEPTION__:message:<<NO_METHOD_IN_CLASS noSuchMethod[0] TestSlim.>>", result);

	result = StatementExecutor_Call(statementExecutor, "test_slim", "noOtherSuchMethod", args);
	STRCMP_EQUAL("__EXCEPTION__:message:<<NO_METHOD_IN_CLASS noOtherSuchMethod[0] TestSlim.>>", result);
}

TEST(StatementExecutor, shouldTruncateReallyLongNamedFunctionThatDoesNotExistTo32)
{
	char * result = StatementExecutor_Call(statementExecutor, "test_slim", "noOtherSuchMethod123456789022345678903234567890423456789052345678906234567890", args);
	CHECK(strlen(result) < 120);
	STRCMP_EQUAL("__EXCEPTION__:message:<<NO_METHOD_IN_CLASS noOtherSuchMethod123456789022345[0] TestSlim.>>", result);	
}

TEST(StatementExecutor, shouldKnowNumberofArgumentsforNonExistantFunction)
{
	SlimList_AddString(args, "BlahBlah");
	char* result = StatementExecutor_Call(statementExecutor, "test_slim", "noSuchMethod", args);
	STRCMP_EQUAL("__EXCEPTION__:message:<<NO_METHOD_IN_CLASS noSuchMethod[1] TestSlim.>>", result);
}

TEST(StatementExecutor, shouldNotAllowACallToaNonexistentInstance)
{
	char* result = StatementExecutor_Call(statementExecutor, "noSuchInstance", "noArgs", args);
	STRCMP_EQUAL("__EXCEPTION__:message:<<NO_INSTANCE noSuchInstance.>>", result);	
}

TEST(StatementExecutor, shouldNotAllowAMakeOnANonexistentClass)
{
	char * result = StatementExecutor_Make(statementExecutor, "instanceName", "NoSuchClass", empty);
	STRCMP_EQUAL("__EXCEPTION__:message:<<NO_CLASS NoSuchClass.>>", result);	
}

TEST(StatementExecutor, canCallaMethodThatReturnsAValue) {
	char* result = StatementExecutor_Call(statementExecutor, "test_slim", "returnValue", args);
	STRCMP_EQUAL("value", result);
}

TEST(StatementExecutor, canCallaMethodThatTakesASlimList)
{

	SlimList_AddString(args, "hello world");
	
	char* result = StatementExecutor_Call(statementExecutor, "test_slim", "echo", args);
	STRCMP_EQUAL("hello world", result);
}

TEST(StatementExecutor, WhereCalledFunctionHasUnderscoresSeparatingNameParts)
{

	SlimList_AddString(args, "hello world");

	StatementExecutor_Call(statementExecutor, "test_slim", "setArg", args);
	char* result = StatementExecutor_Call(statementExecutor, "test_slim", "getArgFromFunctionWithUnderscores", empty);
	STRCMP_EQUAL("hello world", result);
}

TEST(StatementExecutor, canCallTwoInstancesOfTheSameFixture)
{
	SlimList* args2 = SlimList_Create();
	SlimList_AddString(args, "one");
	SlimList_AddString(args2, "two");
	
	StatementExecutor_Make(statementExecutor, "test_slim2", "TestSlim", empty);
	StatementExecutor_Call(statementExecutor, "test_slim", "setArg", args);
	StatementExecutor_Call(statementExecutor, "test_slim2", "setArg", args2);
	char* one = StatementExecutor_Call(statementExecutor, "test_slim", "getArg", empty);
	char* two = StatementExecutor_Call(statementExecutor, "test_slim2", "getArg", empty);
	STRCMP_EQUAL("one", one);
	STRCMP_EQUAL("two", two);
	SlimList_Destroy(args2);
}

TEST(StatementExecutor, canCreateTwoDifferentFixtures)
{
	SlimList* args2 = SlimList_Create();
	SlimList_AddString(args, "one");
	SlimList_AddString(args2, "two");
		
	StatementExecutor_Make(statementExecutor, "test_slim2", "TestSlimAgain", empty);	
	StatementExecutor_Call(statementExecutor, "test_slim", "setArg", args);	
	StatementExecutor_Call(statementExecutor, "test_slim2", "setArgAgain", args2);
	char* one = StatementExecutor_Call(statementExecutor, "test_slim", "getArg", empty);
	char* two = StatementExecutor_Call(statementExecutor, "test_slim2", "getArgAgain", empty);
	STRCMP_EQUAL("one", one);
	STRCMP_EQUAL("two", two);
	SlimList_Destroy(args2);
}
TEST(StatementExecutor, canReplaceSymbolsWithTheirValue)
{
	StatementExecutor_SetSymbol(statementExecutor, "v", "bob");
	SlimList_AddString(args, "hi $v.");
	char * result = StatementExecutor_Call(statementExecutor, "test_slim", "echo", args);
	LONGS_EQUAL(strlen("hi bob."), strlen(result))
    STRCMP_EQUAL("hi bob.", result)   
}

TEST(StatementExecutor, canReplaceSymbolsInTheMiddle)
{
	StatementExecutor_SetSymbol(statementExecutor, "v", "bob");
	SlimList_AddString(args, "hi $v whats up.");
	STRCMP_EQUAL("hi bob whats up.", StatementExecutor_Call(statementExecutor, "test_slim", "echo", args))
}

TEST(StatementExecutor, canReplaceSymbolsWithOtherNonAlphaNumeric)
{
	StatementExecutor_SetSymbol(statementExecutor, "v2", "bob");
	SlimList_AddString(args, "$v2=why");
	STRCMP_EQUAL("bob=why", StatementExecutor_Call(statementExecutor, "test_slim", "echo", args))
}

TEST(StatementExecutor, canReplaceMultipleSymbolsWithTheirValue)
{
	StatementExecutor_SetSymbol(statementExecutor, "v", "bob");
	StatementExecutor_SetSymbol(statementExecutor, "e", "doug");
	SlimList_AddString(args, "hi $v. Cost:  $12.32 from $e.");
    STRCMP_EQUAL("hi bob. Cost:  $12.32 from doug.", StatementExecutor_Call(statementExecutor, "test_slim", "echo", args))   
}

TEST(StatementExecutor, canHandlestringWithJustADollarSign)
{
	StatementExecutor_SetSymbol(statementExecutor, "v2", "bob");
	SlimList_AddString(args, "$");
	STRCMP_EQUAL("$", StatementExecutor_Call(statementExecutor, "test_slim", "echo", args))
}

TEST(StatementExecutor, canHandleDollarSignAtTheEndOfTheString)
{
	StatementExecutor_SetSymbol(statementExecutor, "v2", "doug");
	SlimList_AddString(args, "hi $v2$");
	STRCMP_EQUAL("hi doug$", StatementExecutor_Call(statementExecutor, "test_slim", "echo", args))
}

TEST(StatementExecutor, canReplaceSymbolsInSubLists) 
{
	StatementExecutor_SetSymbol(statementExecutor, "v2", "doug");
	SlimList* subList = SlimList_Create();
	SlimList_AddString(subList, "Hi $v2.");
	SlimList_AddList(args, subList);
	char* result = StatementExecutor_Call(statementExecutor, "test_slim", "echo", args);
	CHECK(result != NULL);
	SlimList* returnedList = SlimList_Deserialize(result);
	CHECK(NULL != returnedList);
	LONGS_EQUAL(1, SlimList_GetLength(returnedList));
	char* element = SlimList_GetStringAt(returnedList, 0);
	STRCMP_EQUAL("Hi doug.", element);
	SlimList_Destroy(subList);	
	SlimList_Destroy(returnedList);
}

TEST(StatementExecutor, canReplaceSymbolsInSubSubLists) 
{
	StatementExecutor_SetSymbol(statementExecutor, "v2", "doug");
	SlimList* subList = SlimList_Create();
	SlimList* subSubList = SlimList_Create();
	SlimList_AddString(subSubList, "Hi $v2.");
	SlimList_AddList(subList, subSubList);
	SlimList_AddList(args, subList);
	char* result = StatementExecutor_Call(statementExecutor, "test_slim", "echo", args);
	CHECK(result != NULL);
	SlimList* returnedSubList = SlimList_Deserialize(result);
	CHECK(NULL != returnedSubList);
	LONGS_EQUAL(1, SlimList_GetLength(returnedSubList));
	SlimList* returnedSubSubList = SlimList_GetListAt(returnedSubList, 0);
	CHECK(NULL != returnedSubSubList);
	LONGS_EQUAL(1, SlimList_GetLength(returnedSubSubList));	
	char* element = SlimList_GetStringAt(returnedSubSubList, 0);
	CHECK(NULL != element);
	STRCMP_EQUAL("Hi doug.", element);
	SlimList_Destroy(subSubList);
	SlimList_Destroy(subList);	
	SlimList_Destroy(returnedSubList);
}

TEST(StatementExecutor, canCreateFixtureWithArguments) 
{
	SlimList* constructionArgs = SlimList_Create();
	SlimList_AddString(constructionArgs, "hi");
	StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", constructionArgs);
	char* result = StatementExecutor_Call(statementExecutor, "test_slim", "getConstructionArg", empty);
	STRCMP_EQUAL("hi", result);
	
	SlimList_Destroy(constructionArgs);
}

TEST(StatementExecutor, canCreateFixtureWithArgumentsThatHaveSymbols) 
{
	StatementExecutor_SetSymbol(statementExecutor, "name", "doug");
	SlimList* constructionArgs = SlimList_Create();
	SlimList_AddString(constructionArgs, "hi $name");
	StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", constructionArgs);
	char* result = StatementExecutor_Call(statementExecutor, "test_slim", "getConstructionArg", empty);
	STRCMP_EQUAL("hi doug", result);
	
	SlimList_Destroy(constructionArgs);
}

TEST(StatementExecutor, canCreateFixtureWithArgumentsThatHaveMultipleSymbols) 
{
	StatementExecutor_SetSymbol(statementExecutor, "fname", "doug");
	StatementExecutor_SetSymbol(statementExecutor, "lname", "bradbury");
	
	SlimList* constructionArgs = SlimList_Create();
	SlimList_AddString(constructionArgs, "hi $fname $lname");
	StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", constructionArgs);
	char* result = StatementExecutor_Call(statementExecutor, "test_slim", "getConstructionArg", empty);
	STRCMP_EQUAL("hi doug bradbury", result);
	
	SlimList_Destroy(constructionArgs);
}

TEST(StatementExecutor, fixtureConstructionFailsWithUserErrorMessage) 
{
	SlimList* constructionArgs = SlimList_Create();
	SlimList_AddString(constructionArgs, "hi doug");
	SlimList_AddString(constructionArgs, "ho doug");
	
	char* result = StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", constructionArgs);
	STRCMP_EQUAL("__EXCEPTION__:message:<<COULD_NOT_INVOKE_CONSTRUCTOR TestSlim xxx.>>", result);
	
	SlimList_Destroy(constructionArgs);
}

TEST(StatementExecutor, fixtureCanReturnError) 
{
	char* result = StatementExecutor_Call(statementExecutor, "test_slim", "returnError", args);
	STRCMP_EQUAL("__EXCEPTION__:message:<<my exception.>>", result);	
}

TEST(StatementExecutor, canCallFixtureDeclaredBackwards) 
{
	StatementExecutor_Make(statementExecutor, "backwardsTestSlim", "TestSlimDeclaredLate", empty);
	SlimList_AddString(args, "hi doug");
	char* result = StatementExecutor_Call(statementExecutor, "backwardsTestSlim", "echo", args);
	STRCMP_EQUAL("hi doug", result);
}

TEST(StatementExecutor, canCallFixtureNotDeclared) 
{
	StatementExecutor_Make(statementExecutor, "undeclaredTestSlim", "TestSlimUndeclared", empty);
	SlimList_AddString(args, "hi doug");
	char* result = StatementExecutor_Call(statementExecutor, "undeclaredTestSlim", "echo", args);
	STRCMP_EQUAL("hi doug", result);
}

TEST(StatementExecutor, canHaveNullResult)
{
	char* result = StatementExecutor_Call(statementExecutor, "test_slim", "null", args);
	POINTERS_EQUAL(0, result);  
}
