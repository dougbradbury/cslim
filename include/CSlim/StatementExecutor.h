#ifndef D_StatementExecutor_H
#define D_StatementExecutor_H

///////////////////////////////////////////////////////////////////////////////
//
//  StatementExecutor is responsible for ...
//
///////////////////////////////////////////////////////////////////////////////
#include "SlimList.h"

typedef struct StatementExecutor StatementExecutor;
typedef void(*Fixture)(StatementExecutor*);
typedef void*(*Constructor)();
typedef void(*Destructor)(void *);
typedef char*(*Method)(void *, SlimList*);
StatementExecutor* StatementExecutor_Create(void);
void StatementExecutor_Destroy(StatementExecutor*);
char* StatementExecutor_make(StatementExecutor*, char* instanceName, char* className);
char* StatementExecutor_call(StatementExecutor*, char* instanceName, char* methodName, SlimList*);
void* StatementExecutor_instance(StatementExecutor*, char* instanceName);
void StatementExecutor_addFixture(StatementExecutor* executor, Fixture);
void StatementExecutor_RegisterFixture(StatementExecutor*, char * className, Constructor, Destructor);
void StatementExecutor_RegisterMethod(StatementExecutor*, char * className, char * methodName, Method);
void StatementExecutor_setSymbol(StatementExecutor*, char* symbol, char* value);
#endif  // D_StatementExecutor_H
