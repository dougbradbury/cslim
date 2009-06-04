#ifndef D_TestSlim_H
#define D_TestSlim_H

///////////////////////////////////////////////////////////////////////////////
//
//  TestSlim is responsible for ...
//
///////////////////////////////////////////////////////////////////////////////
typedef struct TestSlim TestSlim;
#include "StatementExecutor.h"
// #include "SlimList.h"

void* TestSlim_Create(StatementExecutor* executor, SlimList* args);
void TestSlim_Destroy(void*);
void TestSlim_Register(StatementExecutor*);

int TestSlim_noArgsCalled(TestSlim* executor);
#endif  // D_TestSlim_H
