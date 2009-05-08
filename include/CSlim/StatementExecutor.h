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
typedef void*(*Constructor)(StatementExecutor*, SlimList*);
typedef void(*Destructor)(void *);
typedef char*(*Method)(void *, SlimList*);

StatementExecutor* StatementExecutor_Create(void);
void StatementExecutor_Destroy(StatementExecutor*);

void StatementExecutor_AddFixture(StatementExecutor* executor, Fixture);
void StatementExecutor_RegisterFixture(StatementExecutor*, char * className, Constructor, Destructor);
void StatementExecutor_RegisterMethod(StatementExecutor*, char * className, char * methodName, Method);

char* StatementExecutor_Make(StatementExecutor*, char* instanceName, char* className, SlimList* args);
char* StatementExecutor_Call(StatementExecutor*, char* instanceName, char* methodName, SlimList*);
void* StatementExecutor_Instance(StatementExecutor*, char* instanceName);
void StatementExecutor_SetSymbol(StatementExecutor*, char* symbol, char* value);

void StatementExecutor_ConstructorError(StatementExecutor* executor, char* message);
char* StatementExecutor_FixtureError(char* message);

#endif  // D_StatementExecutor_H
