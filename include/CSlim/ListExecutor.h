#ifndef D_ListExecutor_H
#define D_ListExecutor_H

#ifndef CPP_COMPILING
#ifdef __cplusplus
extern "C" {
#endif
#endif

#include "SlimList.h"
#include "StatementExecutor.h"
typedef struct ListExecutor ListExecutor;

ListExecutor* ListExecutor_Create(StatementExecutor*);
void ListExecutor_Destroy(ListExecutor*);
SlimList* ListExecutor_Execute(ListExecutor*, SlimList*);

#ifndef CPP_COMPILING
#ifdef __cplusplus
}
#endif
#endif

#endif
