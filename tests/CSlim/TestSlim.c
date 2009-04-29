#include "TestSlim.h"
#include <stdlib.h>
#include <memory.h>


//static local variables
struct TestSlim
{
    int noArgsCalled;
	char* arg;
};

void* TestSlim_Create(void)
{
     TestSlim* self = malloc(sizeof(TestSlim));
     memset(self, 0, sizeof(TestSlim));
     return self;
}

void TestSlim_Destroy(void* self)
{
    free(self);
}

int TestSlim_noArgsCalled(TestSlim* executor) {
	return executor->noArgsCalled;		
}

char* noArgs(void* self, SlimList* args) {
	TestSlim* me = (TestSlim*)self;
	me->noArgsCalled = 1;
	return "/__VOID__/";
}

char* returnValue(void* self, SlimList* args) {
	return "value";
}

char* oneArg(void* self, SlimList* args) {
	return SlimList_getStringAt(args, 0);
}

char* setArg(void* self, SlimList* args) {
	TestSlim* me = (TestSlim*)self;
	me->arg = SlimList_getStringAt(args, 0);
	return "/__VOID__/";
}

char* getArg(void* self, SlimList* args) {
	TestSlim* me = (TestSlim*)self;
	return me->arg;
}

void TestSlim_Register(StatementExecutor* executor)
{
	StatementExecutor_RegisterFixture(executor, "TestSlim", TestSlim_Create, TestSlim_Destroy);
	StatementExecutor_RegisterMethod(executor, "TestSlim", "returnValue", returnValue);	
	StatementExecutor_RegisterMethod(executor, "TestSlim", "noArgs", noArgs);
	StatementExecutor_RegisterMethod(executor, "TestSlim", "oneArg", oneArg);
	StatementExecutor_RegisterMethod(executor, "TestSlim", "setArg", setArg);
	StatementExecutor_RegisterMethod(executor, "TestSlim", "getArg", getArg);
	
	StatementExecutor_RegisterFixture(executor, "TestSlimAgain", TestSlim_Create, TestSlim_Destroy);
	StatementExecutor_RegisterMethod(executor, "TestSlimAgain", "setArgAgain", setArg);
	StatementExecutor_RegisterMethod(executor, "TestSlimAgain", "getArgAgain", getArg);
}