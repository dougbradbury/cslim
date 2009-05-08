#include "TestSlim.h"
#include <stdlib.h>
#include <memory.h>


//static local variables
struct TestSlim
{
    int noArgsCalled;
	char* arg;
	SlimList* constructionArgs;
};

void* TestSlim_Create(StatementExecutor* executor, SlimList* args)
{
	if (SlimList_GetLength(args) > 1) {
		StatementExecutor_ConstructorError(executor, "xxx");
		return NULL;
	}

	TestSlim* self = malloc(sizeof(TestSlim));
	memset(self, 0, sizeof(TestSlim));
	self->constructionArgs = args;
	return self;
}

void TestSlim_Destroy(void* self)
{
    free(self);
}

int TestSlim_noArgsCalled(TestSlim* executor) {
	return executor->noArgsCalled;		
}

static char* noArgs(void* self, SlimList* args) {
	TestSlim* me = (TestSlim*)self;
	me->noArgsCalled = 1;
	return "/__VOID__/";
}

static char* returnValue(void* self, SlimList* args) {
	return "value";
}

static char* oneArg(void* self, SlimList* args) {
	return SlimList_GetStringAt(args, 0);
}

static char* setArg(void* self, SlimList* args) {
	TestSlim* me = (TestSlim*)self;
	me->arg = SlimList_GetStringAt(args, 0);
	return "/__VOID__/";
}
 
static char* getArg(void* self, SlimList* args) {
	TestSlim* me = (TestSlim*)self;
	return me->arg;
}

static char* getConstructionArg(void* self, SlimList* args) {
	TestSlim* me = (TestSlim*)self;	
	return SlimList_GetStringAt(me->constructionArgs, 0);
}

static char* returnError(void* self, SlimList* args) {
	return StatementExecutor_FixtureError("my exception");
}

void TestSlim_Register(StatementExecutor* executor)
{
	StatementExecutor_RegisterFixture(executor, "TestSlim", TestSlim_Create, TestSlim_Destroy);
	StatementExecutor_RegisterMethod(executor, "TestSlim", "returnValue", returnValue);	
	StatementExecutor_RegisterMethod(executor, "TestSlim", "noArgs", noArgs);
	StatementExecutor_RegisterMethod(executor, "TestSlim", "echo", oneArg);
	StatementExecutor_RegisterMethod(executor, "TestSlim", "setArg", setArg);
	StatementExecutor_RegisterMethod(executor, "TestSlim", "getArg", getArg);
	StatementExecutor_RegisterMethod(executor, "TestSlim", "getConstructionArg", getConstructionArg);
	StatementExecutor_RegisterMethod(executor, "TestSlim", "returnError", returnError);
	
	
	StatementExecutor_RegisterFixture(executor, "TestSlimAgain", TestSlim_Create, TestSlim_Destroy);
	StatementExecutor_RegisterMethod(executor, "TestSlimAgain", "setArgAgain", setArg);
	StatementExecutor_RegisterMethod(executor, "TestSlimAgain", "getArgAgain", getArg);
	
	StatementExecutor_RegisterMethod(executor, "TestSlimDeclaredLate", "echo", oneArg);
	StatementExecutor_RegisterFixture(executor, "TestSlimDeclaredLate", TestSlim_Create, TestSlim_Destroy);
	
	StatementExecutor_RegisterMethod(executor, "TestSlimUndeclared", "echo", oneArg);
}