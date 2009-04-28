#ifndef D_TestSlim_H
#define D_TestSlim_H

///////////////////////////////////////////////////////////////////////////////
//
//  TestSlim is responsible for ...
//
///////////////////////////////////////////////////////////////////////////////
typedef struct TestSlim TestSlim;
#include "StatementExecutor.h"

void* TestSlim_Create(void);
void TestSlim_Destroy(void*);
void TestSlim_Register(StatementExecutor*);

int TestSlim_noArgsCalled(TestSlim* executor);
#endif  // D_TestSlim_H
