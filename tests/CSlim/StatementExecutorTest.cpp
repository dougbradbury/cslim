#include "CppUTest/TestHarness.h"

extern "C"
{
#include "StatementExecutor.h"
#include "TestSlim.h"
#include "SlimList.h"
#include "SlimListDeserializer.h"
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
	char * result = StatementExecutor_call(statementExecutor, "test_slim", "echo", args);
	LONGS_EQUAL(strlen("hi bob."), strlen(result))
    STRCMP_EQUAL("hi bob.", result)   
}

TEST(StatementExecutor, canReplaceSymbolsInTheMiddle)
{
	StatementExecutor_setSymbol(statementExecutor, "v", "bob");
	SlimList_addString(args, "hi $v whats up.");
	STRCMP_EQUAL("hi bob whats up.", StatementExecutor_call(statementExecutor, "test_slim", "echo", args))
}

TEST(StatementExecutor, canReplaceSymbolsWithOtherNonAlphaNumeric)
{
	StatementExecutor_setSymbol(statementExecutor, "v2", "bob");
	SlimList_addString(args, "$v2=why");
	STRCMP_EQUAL("bob=why", StatementExecutor_call(statementExecutor, "test_slim", "echo", args))
}

TEST(StatementExecutor, canReplaceMultipleSymbolsWithTheirValue)
{
	StatementExecutor_setSymbol(statementExecutor, "v", "bob");
	StatementExecutor_setSymbol(statementExecutor, "e", "doug");
	SlimList_addString(args, "hi $v. Cost:  $12.32 from $e.");
    STRCMP_EQUAL("hi bob. Cost:  $12.32 from doug.", StatementExecutor_call(statementExecutor, "test_slim", "echo", args))   
}

TEST(StatementExecutor, canHandlestringWithJustADollarSign)
{
	StatementExecutor_setSymbol(statementExecutor, "v2", "bob");
	SlimList_addString(args, "$");
	STRCMP_EQUAL("$", StatementExecutor_call(statementExecutor, "test_slim", "echo", args))
}

TEST(StatementExecutor, canHandleDollarSignAtTheEndOfTheString)
{
	StatementExecutor_setSymbol(statementExecutor, "v2", "doug");
	SlimList_addString(args, "hi $v2$");
	STRCMP_EQUAL("hi doug$", StatementExecutor_call(statementExecutor, "test_slim", "echo", args))
}

TEST(StatementExecutor, canReplaceSymbolsInSubLists) 
{
	StatementExecutor_setSymbol(statementExecutor, "v2", "doug");
	SlimList* subList = SlimList_create();
	SlimList_addString(subList, "Hi $v2.");
	SlimList_addList(args, subList);
	char* result = StatementExecutor_call(statementExecutor, "test_slim", "echo", args);
	CHECK(result != NULL);
	SlimList* returnedList = SlimList_deserialize(result);
	CHECK(NULL != returnedList);
	LONGS_EQUAL(1, SlimList_getLength(returnedList));
	char* element = SlimList_getStringAt(returnedList, 0);
	STRCMP_EQUAL("Hi doug.", element);
	SlimList_destroy(subList);	
	SlimList_destroy(returnedList);
}

TEST(StatementExecutor, canReplaceSymbolsInSubSubLists) 
{
	StatementExecutor_setSymbol(statementExecutor, "v2", "doug");
	SlimList* subList = SlimList_create();
	SlimList* subSubList = SlimList_create();
	SlimList_addString(subSubList, "Hi $v2.");
	SlimList_addList(subList, subSubList);
	SlimList_addList(args, subList);
	char* result = StatementExecutor_call(statementExecutor, "test_slim", "echo", args);
	CHECK(result != NULL);
	SlimList* returnedSubList = SlimList_deserialize(result);
	CHECK(NULL != returnedSubList);
	LONGS_EQUAL(1, SlimList_getLength(returnedSubList));
	SlimList* returnedSubSubList = SlimList_getListAt(returnedSubList, 0);
	CHECK(NULL != returnedSubSubList);
	LONGS_EQUAL(1, SlimList_getLength(returnedSubSubList));	
	char* element = SlimList_getStringAt(returnedSubSubList, 0);
	CHECK(NULL != element);
	STRCMP_EQUAL("Hi doug.", element);
	SlimList_destroy(subSubList);
	SlimList_destroy(subList);	
	SlimList_destroy(returnedSubList);
}



