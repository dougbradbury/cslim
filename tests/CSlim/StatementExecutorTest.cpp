#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "SlimList.h"
#include "SlimListDeserializer.h"
#include "StatementExecutor.h"
#include "TestSlim.h"

#include <stdio.h>
#include <string.h>

TEST_GROUP(StatementExecutor)
{
  StatementExecutor* statementExecutor;
  SlimList*          args;
  SlimList*          empty;

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
  const char* result = StatementExecutor_Call(statementExecutor, "test_slim", "noSuchMethod", args);
  STRCMP_EQUAL("__EXCEPTION__:message:<<NO_METHOD_IN_CLASS noSuchMethod[0] TestSlim.>>", result);

  result = StatementExecutor_Call(statementExecutor, "test_slim", "noOtherSuchMethod", args);
  STRCMP_EQUAL("__EXCEPTION__:message:<<NO_METHOD_IN_CLASS noOtherSuchMethod[0] TestSlim.>>",
               result);
}

TEST(StatementExecutor, shouldTruncateReallyLongNamedFunctionThatDoesNotExistTo32)
{
  const char* result = StatementExecutor_Call(
      statementExecutor, "test_slim",
      "noOtherSuchMethod123456789022345678903234567890423456789052345678906234567890", args);
  CHECK(strlen(result) < 120);
  STRCMP_EQUAL(
      "__EXCEPTION__:message:<<NO_METHOD_IN_CLASS noOtherSuchMethod123456789022345[0] TestSlim.>>",
      result);
}

TEST(StatementExecutor, shouldKnowNumberofArgumentsforNonExistantFunction)
{
  SlimList_AddString(args, "BlahBlah");
  const char* result = StatementExecutor_Call(statementExecutor, "test_slim", "noSuchMethod", args);
  STRCMP_EQUAL("__EXCEPTION__:message:<<NO_METHOD_IN_CLASS noSuchMethod[1] TestSlim.>>", result);
}

TEST(StatementExecutor, shouldNotAllowACallToaNonexistentInstance)
{
  const char* result = StatementExecutor_Call(statementExecutor, "noSuchInstance", "noArgs", args);
  STRCMP_EQUAL("__EXCEPTION__:message:<<NO_INSTANCE noSuchInstance.>>", result);
}

TEST(StatementExecutor, shouldNotAllowAMakeOnANonexistentClass)
{
  const char* result =
      StatementExecutor_Make(statementExecutor, "instanceName", "NoSuchClass", empty);
  STRCMP_EQUAL("__EXCEPTION__:message:<<NO_CLASS NoSuchClass.>>", result);
}

TEST(StatementExecutor, canCallaMethodThatReturnsAValue)
{
  const char* result = StatementExecutor_Call(statementExecutor, "test_slim", "returnValue", args);
  STRCMP_EQUAL("value", result);
}

TEST(StatementExecutor, canCallaMethodThatTakesASlimList)
{

  SlimList_AddString(args, "hello world");

  const char* result = StatementExecutor_Call(statementExecutor, "test_slim", "echo", args);
  STRCMP_EQUAL("hello world", result);
}

TEST(StatementExecutor, WhereCalledFunctionHasUnderscoresSeparatingNameParts)
{

  SlimList_AddString(args, "hello world");

  StatementExecutor_Call(statementExecutor, "test_slim", "setArg", args);
  const char* result = StatementExecutor_Call(statementExecutor, "test_slim",
                                              "getArgFromFunctionWithUnderscores", empty);
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
  const char* one = StatementExecutor_Call(statementExecutor, "test_slim", "getArg", empty);
  const char* two = StatementExecutor_Call(statementExecutor, "test_slim2", "getArg", empty);
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
  const char* one = StatementExecutor_Call(statementExecutor, "test_slim", "getArg", empty);
  const char* two = StatementExecutor_Call(statementExecutor, "test_slim2", "getArgAgain", empty);
  STRCMP_EQUAL("one", one);
  STRCMP_EQUAL("two", two);
  SlimList_Destroy(args2);
}
TEST(StatementExecutor, canReplaceSymbolsWithTheirValue)
{
  StatementExecutor_SetSymbol(statementExecutor, "v", "bob");
  SlimList_AddString(args, "hi $v.");
  const char* result = StatementExecutor_Call(statementExecutor, "test_slim", "echo", args);
  LONGS_EQUAL(strlen("hi bob."), strlen(result))
  STRCMP_EQUAL("hi bob.", result)
}

TEST(StatementExecutor, canReplaceSymbolsInTheMiddle)
{
  StatementExecutor_SetSymbol(statementExecutor, "v", "bob");
  SlimList_AddString(args, "hi $v whats up.");
  STRCMP_EQUAL("hi bob whats up.",
               StatementExecutor_Call(statementExecutor, "test_slim", "echo", args))
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
  STRCMP_EQUAL("hi bob. Cost:  $12.32 from doug.",
               StatementExecutor_Call(statementExecutor, "test_slim", "echo", args))
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
  const char* result = StatementExecutor_Call(statementExecutor, "test_slim", "echo", args);
  CHECK(result != NULL);
  SlimList* returnedList = SlimList_Deserialize(result);
  CHECK(NULL != returnedList);
  LONGS_EQUAL(1, SlimList_GetLength(returnedList));
  const char* element = SlimList_GetStringAt(returnedList, 0);
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
  const char* result = StatementExecutor_Call(statementExecutor, "test_slim", "echo", args);
  CHECK(result != NULL);
  SlimList* returnedSubList = SlimList_Deserialize(result);
  CHECK(NULL != returnedSubList);
  LONGS_EQUAL(1, SlimList_GetLength(returnedSubList));
  SlimList* returnedSubSubList = SlimList_GetListAt(returnedSubList, 0);
  CHECK(NULL != returnedSubSubList);
  LONGS_EQUAL(1, SlimList_GetLength(returnedSubSubList));
  const char* element = SlimList_GetStringAt(returnedSubSubList, 0);
  CHECK(NULL != element);
  STRCMP_EQUAL("Hi doug.", element);
  SlimList_Destroy(subSubList);
  SlimList_Destroy(subList);
  SlimList_Destroy(returnedSubList);
}

TEST(StatementExecutor, canCreateFixtureWithSymbolAsClassName)
{
  StatementExecutor_SetSymbol(statementExecutor, "fixtureName", "Test_Slim");
  const char* makeResponse =
      StatementExecutor_Make(statementExecutor, "instanceName", "$fixtureName", empty);
  STRCMP_EQUAL("OK", makeResponse);
}

TEST(StatementExecutor, shouldNotAllowAMakeOnANonexistentClassReferencedBySymbol)
{
  StatementExecutor_SetSymbol(statementExecutor, "fixtureName", "NoSuchClass");
  const char* makeResponse =
      StatementExecutor_Make(statementExecutor, "instanceName", "$fixtureName", empty);
  STRCMP_EQUAL("__EXCEPTION__:message:<<NO_CLASS $fixtureName.>>", makeResponse);
}

TEST(StatementExecutor, canCreateFixtureWithSymbolInClassName)
{
  StatementExecutor_SetSymbol(statementExecutor, "test", "Test");
  const char* makeResponse =
      StatementExecutor_Make(statementExecutor, "instanceName", "$test_Slim", empty);
  STRCMP_EQUAL("OK", makeResponse);
}

TEST(StatementExecutor, canCreateFixtureWithMultipleSymbolsInClassName)
{
  StatementExecutor_SetSymbol(statementExecutor, "test", "Test");
  StatementExecutor_SetSymbol(statementExecutor, "slim", "Slim");
  const char* makeResponse =
      StatementExecutor_Make(statementExecutor, "instanceName", "$test_$slim", empty);
  STRCMP_EQUAL("OK", makeResponse);
}

TEST(StatementExecutor, canCreateFixtureWithArguments)
{
  SlimList* constructionArgs = SlimList_Create();
  SlimList_AddString(constructionArgs, "hi");
  StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", constructionArgs);
  const char* result =
      StatementExecutor_Call(statementExecutor, "test_slim", "getConstructionArg", empty);
  STRCMP_EQUAL("hi", result);

  SlimList_Destroy(constructionArgs);
}

TEST(StatementExecutor, canCreateFixtureWithArgumentsThatHaveSymbols)
{
  StatementExecutor_SetSymbol(statementExecutor, "name", "doug");
  SlimList* constructionArgs = SlimList_Create();
  SlimList_AddString(constructionArgs, "hi $name");
  StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", constructionArgs);
  const char* result =
      StatementExecutor_Call(statementExecutor, "test_slim", "getConstructionArg", empty);
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
  const char* result =
      StatementExecutor_Call(statementExecutor, "test_slim", "getConstructionArg", empty);
  STRCMP_EQUAL("hi doug bradbury", result);

  SlimList_Destroy(constructionArgs);
}

TEST(StatementExecutor, fixtureConstructionFailsWithUserErrorMessage)
{
  SlimList* constructionArgs = SlimList_Create();
  SlimList_AddString(constructionArgs, "hi doug");
  SlimList_AddString(constructionArgs, "ho doug");

  const char* result =
      StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", constructionArgs);
  STRCMP_EQUAL("__EXCEPTION__:message:<<COULD_NOT_INVOKE_CONSTRUCTOR TestSlim xxx.>>", result);

  SlimList_Destroy(constructionArgs);
}

TEST(StatementExecutor, fixtureReferencedBySymbolConstructionFailsWithUserErrorMessage)
{
  StatementExecutor_SetSymbol(statementExecutor, "fixtureName", "Test_Slim");
  SlimList_AddString(args, "arg0");
  SlimList_AddString(args, "arg1");
  const char* makeResponse =
      StatementExecutor_Make(statementExecutor, "instanceName", "$fixtureName", args);
  STRCMP_EQUAL("__EXCEPTION__:message:<<COULD_NOT_INVOKE_CONSTRUCTOR $fixtureName xxx.>>",
               makeResponse);
}

TEST(StatementExecutor, fixtureCanReturnError)
{
  const char* result = StatementExecutor_Call(statementExecutor, "test_slim", "returnError", args);
  STRCMP_EQUAL("__EXCEPTION__:message:<<my exception.>>", result);
}

TEST(StatementExecutor, canCallFixtureDeclaredBackwards)
{
  StatementExecutor_Make(statementExecutor, "backwardsTestSlim", "TestSlimDeclaredLate", empty);
  SlimList_AddString(args, "hi doug");
  const char* result = StatementExecutor_Call(statementExecutor, "backwardsTestSlim", "echo", args);
  STRCMP_EQUAL("hi doug", result);
}

TEST(StatementExecutor, canCallFixtureNotDeclared)
{
  StatementExecutor_Make(statementExecutor, "undeclaredTestSlim", "TestSlimUndeclared", empty);
  SlimList_AddString(args, "hi doug");
  const char* result =
      StatementExecutor_Call(statementExecutor, "undeclaredTestSlim", "echo", args);
  STRCMP_EQUAL("hi doug", result);
}

TEST(StatementExecutor, canHaveNullResult)
{
  const char* result = StatementExecutor_Call(statementExecutor, "test_slim", "null", args);
  POINTERS_EQUAL(0, result);
}

TEST_GROUP(StatementExecutorWithLibraryInstances)
{
  StatementExecutor* statementExecutor;
  SlimList*          noArgs;

  void setup()
  {
    statementExecutor = StatementExecutor_Create();
    noArgs = SlimList_Create();
    StatementExecutor_AddFixture(statementExecutor, &RegisterMockFixtureWith1Method);
    StatementExecutor_AddFixture(statementExecutor, &RegisterMockFixtureWith2Methods);
    StatementExecutor_AddFixture(statementExecutor, &RegisterMockFixtureWith3Methods);
  }

  void teardown()
  {
    mock().clear();
    SlimList_Destroy(noArgs);
    StatementExecutor_Destroy(statementExecutor);
  }

  struct MockFixture
  {
    char* method1(SlimList* args)
    {
      return const_cast<char*>(mock()
                                   .actualCall("method1")
                                   .onObject(this)
                                   .withParameter("args", args)
                                   .returnStringValue());
    }

    char* method2(SlimList* args)
    {
      return const_cast<char*>(mock()
                                   .actualCall("method2")
                                   .onObject(this)
                                   .withParameter("args", args)
                                   .returnStringValue());
    }

    char* method3(SlimList* args)
    {
      return const_cast<char*>(mock()
                                   .actualCall("method3")
                                   .onObject(this)
                                   .withParameter("args", args)
                                   .returnStringValue());
    }
  };

  static void* createMockFixture(StatementExecutor*, SlimList*)
  {
    return mock().actualCall("createMockFixture").returnPointerValue();
  }

  static void destroyMockFixture(void* mockFixture)
  {
    delete reinterpret_cast<MockFixture*>(mockFixture);
  }

  static const char* invokeMethod1(void* mockFixture, SlimList* args)
  {
    return reinterpret_cast<MockFixture*>(mockFixture)->method1(args);
  }

  static const char* invokeMethod2(void* mockFixture, SlimList* args)
  {
    return reinterpret_cast<MockFixture*>(mockFixture)->method2(args);
  }

  static const char* invokeMethod3(void* mockFixture, SlimList* args)
  {
    return reinterpret_cast<MockFixture*>(mockFixture)->method3(args);
  }

  static void RegisterMockFixtureWith1Method(StatementExecutor * statementExecutor)
  {
    StatementExecutor_RegisterFixture(statementExecutor, "MockFixtureWith1Method",
                                      &createMockFixture, &destroyMockFixture);
    StatementExecutor_RegisterMethod(statementExecutor, "MockFixtureWith1Method", "method1",
                                     &invokeMethod1);
  }

  static void RegisterMockFixtureWith2Methods(StatementExecutor * statementExecutor)
  {
    StatementExecutor_RegisterFixture(statementExecutor, "MockFixtureWith2Methods",
                                      &createMockFixture, &destroyMockFixture);
    StatementExecutor_RegisterMethod(statementExecutor, "MockFixtureWith2Methods", "method1",
                                     &invokeMethod1);
    StatementExecutor_RegisterMethod(statementExecutor, "MockFixtureWith2Methods", "method2",
                                     &invokeMethod2);
  }

  static void RegisterMockFixtureWith3Methods(StatementExecutor * statementExecutor)
  {
    StatementExecutor_RegisterFixture(statementExecutor, "MockFixtureWith3Methods",
                                      &createMockFixture, &destroyMockFixture);
    StatementExecutor_RegisterMethod(statementExecutor, "MockFixtureWith3Methods", "method1",
                                     &invokeMethod1);
    StatementExecutor_RegisterMethod(statementExecutor, "MockFixtureWith3Methods", "method2",
                                     &invokeMethod2);
    StatementExecutor_RegisterMethod(statementExecutor, "MockFixtureWith3Methods", "method3",
                                     &invokeMethod3);
  }
};

TEST(StatementExecutorWithLibraryInstances, callsMethodOnInstanceFirst)
{
  MockFixture* standardInstance = new MockFixture;
  MockFixture* libraryInstance = new MockFixture;

  mock().expectOneCall("createMockFixture").andReturnValue(standardInstance);
  mock().expectOneCall("createMockFixture").andReturnValue(libraryInstance);
  mock()
      .expectOneCall("method1")
      .onObject(standardInstance)
      .withParameter("args", noArgs)
      .andReturnValue("OK");

  StatementExecutor_Make(statementExecutor, "standardInstance", "MockFixtureWith1Method", noArgs);
  StatementExecutor_Make(statementExecutor, "libraryInstance", "MockFixtureWith2Methods", noArgs);
  const char* result =
      StatementExecutor_Call(statementExecutor, "standardInstance", "method1", noArgs);

  STRCMP_EQUAL("OK", result);
  mock().checkExpectations();
}

TEST(StatementExecutorWithLibraryInstances, callsMethodOnLibraryInstanceWhenNotFoundOnGivenInstance)
{
  MockFixture* standardInstance = new MockFixture;
  MockFixture* libraryInstance = new MockFixture;

  mock().expectOneCall("createMockFixture").andReturnValue(standardInstance);
  mock().expectOneCall("createMockFixture").andReturnValue(libraryInstance);
  mock()
      .expectOneCall("method2")
      .onObject(libraryInstance)
      .withParameter("args", noArgs)
      .andReturnValue("OK");

  StatementExecutor_Make(statementExecutor, "standardInstance", "MockFixtureWith1Method", noArgs);
  StatementExecutor_Make(statementExecutor, "libraryInstance", "MockFixtureWith2Methods", noArgs);
  const char* result =
      StatementExecutor_Call(statementExecutor, "standardInstance", "method2", noArgs);

  STRCMP_EQUAL("OK", result);
  mock().checkExpectations();
}

TEST(StatementExecutorWithLibraryInstances,
     callsMethodOnTopOfLibraryInstanceStackWhenNotFoundOnGivenInstance)
{
  MockFixture* standardInstance = new MockFixture;
  MockFixture* libraryInstanceA = new MockFixture;
  MockFixture* libraryInstanceB = new MockFixture;

  mock().expectOneCall("createMockFixture").andReturnValue(standardInstance);
  mock().expectOneCall("createMockFixture").andReturnValue(libraryInstanceA);
  mock().expectOneCall("createMockFixture").andReturnValue(libraryInstanceB);
  mock()
      .expectOneCall("method2")
      .onObject(libraryInstanceB)
      .withParameter("args", noArgs)
      .andReturnValue("OK");

  StatementExecutor_Make(statementExecutor, "standardInstance", "MockFixtureWith1Method", noArgs);
  StatementExecutor_Make(statementExecutor, "libraryInstanceA", "MockFixtureWith3Methods", noArgs);
  StatementExecutor_Make(statementExecutor, "libraryInstanceB", "MockFixtureWith2Methods", noArgs);
  const char* result =
      StatementExecutor_Call(statementExecutor, "standardInstance", "method2", noArgs);

  STRCMP_EQUAL("OK", result);
  mock().checkExpectations();
}

TEST(StatementExecutorWithLibraryInstances,
     callsMethodOnBottomOfLibraryInstanceStackWhenNotFoundOnGivenInstance)
{
  MockFixture* standardInstance = new MockFixture;
  MockFixture* libraryInstanceA = new MockFixture;
  MockFixture* libraryInstanceB = new MockFixture;

  mock().expectOneCall("createMockFixture").andReturnValue(standardInstance);
  mock().expectOneCall("createMockFixture").andReturnValue(libraryInstanceA);
  mock().expectOneCall("createMockFixture").andReturnValue(libraryInstanceB);
  mock()
      .expectOneCall("method3")
      .onObject(libraryInstanceA)
      .withParameter("args", noArgs)
      .andReturnValue("OK");

  StatementExecutor_Make(statementExecutor, "standardInstance", "MockFixtureWith1Method", noArgs);
  StatementExecutor_Make(statementExecutor, "libraryInstanceA", "MockFixtureWith3Methods", noArgs);
  StatementExecutor_Make(statementExecutor, "libraryInstanceB", "MockFixtureWith2Methods", noArgs);
  const char* result =
      StatementExecutor_Call(statementExecutor, "standardInstance", "method3", noArgs);

  STRCMP_EQUAL("OK", result);
  mock().checkExpectations();
}

TEST(StatementExecutorWithLibraryInstances,
     callMethodThatDoesNotExistOnGivenInstanceOrLibraryInstancesReturnsException)
{
  MockFixture* standardInstance = new MockFixture;
  MockFixture* libraryInstanceA = new MockFixture;
  MockFixture* libraryInstanceB = new MockFixture;

  mock().expectOneCall("createMockFixture").andReturnValue(standardInstance);
  mock().expectOneCall("createMockFixture").andReturnValue(libraryInstanceA);
  mock().expectOneCall("createMockFixture").andReturnValue(libraryInstanceB);

  StatementExecutor_Make(statementExecutor, "standardInstance", "MockFixtureWith1Method", noArgs);
  StatementExecutor_Make(statementExecutor, "libraryInstanceA", "MockFixtureWith3Methods", noArgs);
  StatementExecutor_Make(statementExecutor, "libraryInstanceB", "MockFixtureWith2Methods", noArgs);
  const char* result =
      StatementExecutor_Call(statementExecutor, "standardInstance", "method4", noArgs);

  STRCMP_EQUAL("__EXCEPTION__:message:<<NO_METHOD_IN_CLASS method4[0] MockFixtureWith1Method.>>",
               result);
  mock().checkExpectations();
}
