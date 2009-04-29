#include "CppUTest/TestHarness.h"

extern "C"
{
#include "StatementExecutor.h"
#include "TestSlim.h"
#include "SlimList.h"
#include <string.h>
}

TEST_GROUP(StatementExecutor)
{
    StatementExecutor* statementExecutor;
	SlimList* args;
	SlimList* empty;

    void setup()
    {
		statementExecutor = StatementExecutor_create();
		StatementExecutor_addFixture(statementExecutor, TestSlim_Register);
		StatementExecutor_make(statementExecutor, "test_slim", "TestSlim");
		args = SlimList_create();
		empty = SlimList_create();
    }
    
    void teardown()
    {
		StatementExecutor_destroy(statementExecutor);
		SlimList_destroy(args);
		SlimList_destroy(empty);
    }
};

TEST(StatementExecutor, canCallFunctionWithNoArguments)
{
	StatementExecutor_call(statementExecutor, "test_slim", "noArgs", args);
	TestSlim* testSlim = (TestSlim*)StatementExecutor_instance(statementExecutor, "test_slim");
	CHECK(TestSlim_noArgsCalled(testSlim));
}

TEST(StatementExecutor, cantCallFunctionThatDoesNotExist)
{
	char* result = StatementExecutor_call(statementExecutor, "test_slim", "noSuchMethod", args);
	STRCMP_EQUAL("__EXCEPTION__:message:<<NO_METHOD_IN_CLASS noSuchMethod[0] TestSlim.>>", result);

	result = StatementExecutor_call(statementExecutor, "test_slim", "noOtherSuchMethod", args);
	STRCMP_EQUAL("__EXCEPTION__:message:<<NO_METHOD_IN_CLASS noOtherSuchMethod[0] TestSlim.>>", result);
}

TEST(StatementExecutor, shouldTruncateReallyLongNamedFunctionThatDoesNotExistTo32)
{
	char * result = StatementExecutor_call(statementExecutor, "test_slim", "noOtherSuchMethod123456789022345678903234567890423456789052345678906234567890", args);
	CHECK(strlen(result) < 120);
	STRCMP_EQUAL("__EXCEPTION__:message:<<NO_METHOD_IN_CLASS noOtherSuchMethod123456789022345[0] TestSlim.>>", result);	
}

TEST(StatementExecutor, shouldKnowNumberofArgumentsforNonExistantFunction)
{
	SlimList_addString(args, "BlahBlah");
	char* result = StatementExecutor_call(statementExecutor, "test_slim", "noSuchMethod", args);
	STRCMP_EQUAL("__EXCEPTION__:message:<<NO_METHOD_IN_CLASS noSuchMethod[1] TestSlim.>>", result);
}

TEST(StatementExecutor, shouldNotAllowACallToaNonexistentInstance)
{
	char* result = StatementExecutor_call(statementExecutor, "noSuchInstance", "noArgs", args);
	STRCMP_EQUAL("__EXCEPTION__:message:<<NO_INSTANCE noSuchInstance.>>", result);	
}

TEST(StatementExecutor, shouldNotAllowAMakeOnANonexistentClass)
{
	char * result = StatementExecutor_make(statementExecutor, "instanceName", "NoSuchClass");
	STRCMP_EQUAL("__EXCEPTION__:message:<<NO_CLASS NoSuchClass.>>", result);	
}

TEST(StatementExecutor, canCallaMethodThatReturnsAValue) {
	char* result = StatementExecutor_call(statementExecutor, "test_slim", "returnValue", args);
	STRCMP_EQUAL("value", result);
}

TEST(StatementExecutor, canCallaMethodThatTakesASlimList)
{

	SlimList_addString(args, "hello world");
	
	char* result = StatementExecutor_call(statementExecutor, "test_slim", "echo", args);
	STRCMP_EQUAL("hello world", result);
}

TEST(StatementExecutor, canCallTwoInstancesOfTheSameFixture)
{
	SlimList* args2 = SlimList_create();
	SlimList_addString(args, "one");
	SlimList_addString(args2, "two");
	
	StatementExecutor_make(statementExecutor, "test_slim2", "TestSlim");
	StatementExecutor_call(statementExecutor, "test_slim", "setArg", args);
	StatementExecutor_call(statementExecutor, "test_slim2", "setArg", args2);
	char* one = StatementExecutor_call(statementExecutor, "test_slim", "getArg", empty);
	char* two = StatementExecutor_call(statementExecutor, "test_slim2", "getArg", empty);
	STRCMP_EQUAL("one", one);
	STRCMP_EQUAL("two", two);
	SlimList_destroy(args2);
}

TEST(StatementExecutor, canCreateTwoDifferentFixtures)
{
	SlimList* args2 = SlimList_create();
	SlimList_addString(args, "one");
	SlimList_addString(args2, "two");
	
	StatementExecutor_make(statementExecutor, "test_slim2", "TestSlimAgain");
	StatementExecutor_call(statementExecutor, "test_slim", "setArg", args);
	StatementExecutor_call(statementExecutor, "test_slim2", "setArgAgain", args2);
	char* one = StatementExecutor_call(statementExecutor, "test_slim", "getArg", empty);
	char* two = StatementExecutor_call(statementExecutor, "test_slim2", "getArgAgain", empty);
	STRCMP_EQUAL("one", one);
	STRCMP_EQUAL("two", two);
	SlimList_destroy(args2);
}

TEST(StatementExecutor, canReplaceSymbolsWithTheirValue)
{
	StatementExecutor_setSymbol(statementExecutor, "v", "bob");
	SlimList_addString(args, "hi $v.");
    STRCMP_EQUAL("hi bob.", StatementExecutor_call(statementExecutor, "test_slim", "echo", args))   
}

TEST(StatementExecutor, canReplaceMultipleSymbolsWithTheirValue)
{
	StatementExecutor_setSymbol(statementExecutor, "v", "bob");
	StatementExecutor_setSymbol(statementExecutor, "e", "doug");
	SlimList_addString(args, "hi $v. from $e.  Cost:  $12.32");
    STRCMP_EQUAL("hi bob. from doug.  Cost:  $12.32", StatementExecutor_call(statementExecutor, "test_slim", "echo", args))   
}