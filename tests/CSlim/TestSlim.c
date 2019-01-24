#include "TestSlim.h"
#include "compatibility.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//static local variables
struct TestSlim
{
  int         noArgsCalled;
  const char* arg;
  char        constructionArg[50];
  char        echoBuf[50];
};

void* TestSlim_Create(StatementExecutor* executor, SlimList* args)
{
  if (SlimList_GetLength(args) > 1)
  {
    StatementExecutor_ConstructorError(executor, "xxx");
    return NULL;
  }

  TestSlim* self = (TestSlim*)malloc(sizeof(TestSlim));
  memset(self, 0, sizeof(TestSlim));

  if (SlimList_GetLength(args) == 1)
    strncpy(self->constructionArg, SlimList_GetStringAt(args, 0), 50);

  return self;
}

void TestSlim_Destroy(void* self)
{
  free(self);
}

int TestSlim_noArgsCalled(TestSlim* executor)
{
  return executor->noArgsCalled;
}

static const char* noArgs(void* self, SlimList* args)
{
  TestSlim* me = (TestSlim*)self;
  me->noArgsCalled = 1;
  return "/__VOID__/";
}

static const char* returnValue(void* self, SlimList* args)
{
  return "value";
}

static const char* oneArg(void* self, SlimList* args)
{
  const char* value = SlimList_GetStringAt(args, 0);
  return value;
}

static const char* add(void* self, SlimList* args)
{
  static char buf[50];
  snprintf(buf, 50, "%s%s", SlimList_GetStringAt(args, 0), SlimList_GetStringAt(args, 1));
  return buf;
}

static const char* null(void* self, SlimList* args)
{
  return NULL;
}

static const char* setArg(void* self, SlimList* args)
{
  TestSlim* me = (TestSlim*)self;
  me->arg = SlimList_GetStringAt(args, 0);
  return "/__VOID__/";
}

static const char* getArg(void* self, SlimList* args)
{
  TestSlim* me = (TestSlim*)self;
  return me->arg;
}

static const char* getArg_From_Function_With_Underscores(void* self, SlimList* args)
{
  TestSlim* me = (TestSlim*)self;
  return me->arg;
}

static const char* getConstructionArg(void* self, SlimList* args)
{
  TestSlim* me = (TestSlim*)self;
  return me->constructionArg;
}

static const char* returnError(void* self, SlimList* args)
{
  return StatementExecutor_FixtureError("my exception");
}

void TestSlim_Register(StatementExecutor* executor)
{
  StatementExecutor_RegisterFixture(executor, "TestSlim", TestSlim_Create, TestSlim_Destroy);
  StatementExecutor_RegisterMethod(executor, "TestSlim", "returnValue", returnValue);
  StatementExecutor_RegisterMethod(executor, "TestSlim", "noArgs", noArgs);
  StatementExecutor_RegisterMethod(executor, "TestSlim", "echo", oneArg);
  StatementExecutor_RegisterMethod(executor, "TestSlim", "add", add);
  StatementExecutor_RegisterMethod(executor, "TestSlim", "null", null);
  StatementExecutor_RegisterMethod(executor, "TestSlim", "setArg", setArg);
  StatementExecutor_RegisterMethod(executor, "TestSlim", "getArg", getArg);
  StatementExecutor_RegisterMethod(executor, "TestSlim", "getArg_From_Function_With_Underscores",
                                   getArg_From_Function_With_Underscores);
  StatementExecutor_RegisterMethod(executor, "TestSlim", "getConstructionArg", getConstructionArg);
  StatementExecutor_RegisterMethod(executor, "TestSlim", "returnError", returnError);

  StatementExecutor_RegisterFixture(executor, "TestSlimAgain", TestSlim_Create, TestSlim_Destroy);
  StatementExecutor_RegisterMethod(executor, "TestSlimAgain", "setArgAgain", setArg);
  StatementExecutor_RegisterMethod(executor, "TestSlimAgain", "getArgAgain", getArg);

  StatementExecutor_RegisterMethod(executor, "TestSlimDeclaredLate", "echo", oneArg);
  StatementExecutor_RegisterFixture(executor, "TestSlimDeclaredLate", TestSlim_Create,
                                    TestSlim_Destroy);

  StatementExecutor_RegisterMethod(executor, "TestSlimUndeclared", "echo", oneArg);
}
