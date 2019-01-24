#ifndef D_StatementExecutor_H
#define D_StatementExecutor_H

#ifndef CPP_COMPILING
#ifdef __cplusplus
extern "C" {
#endif
#endif

#include "SlimList.h"

typedef struct StatementExecutor StatementExecutor;
typedef void (*Fixture)(StatementExecutor*);
typedef void* (*Constructor)(StatementExecutor*, SlimList*);
typedef void (*Destructor)(void*);
typedef const char* (*Method)(void*, SlimList*);

StatementExecutor* StatementExecutor_Create(void);
void               StatementExecutor_Destroy(StatementExecutor*);

void StatementExecutor_AddFixture(StatementExecutor* executor, Fixture);
void StatementExecutor_RegisterFixture(StatementExecutor*,
                                       char const* className,
                                       Constructor,
                                       Destructor);
void StatementExecutor_RegisterMethod(StatementExecutor*,
                                      char const* className,
                                      char const* methodName,
                                      Method);

const char* StatementExecutor_Make(StatementExecutor*,
                                   char const* instanceName,
                                   char const* className,
                                   SlimList*   args);
const char* StatementExecutor_Call(StatementExecutor*,
                                   char const* instanceName,
                                   char const* methodName,
                                   SlimList*);
void*       StatementExecutor_Instance(StatementExecutor*, char const* instanceName);
void        StatementExecutor_SetSymbol(StatementExecutor*, char const* symbol, char const* value);

void        StatementExecutor_ConstructorError(StatementExecutor* executor, char const* message);
const char* StatementExecutor_FixtureError(char const* message);

#ifndef CPP_COMPILING
#ifdef __cplusplus
}
#endif
#endif

#endif
