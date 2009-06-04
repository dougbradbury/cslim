#include <stdlib.h>
#include <memory.h>
#include "StatementExecutor.h"
#include "SlimList.h"
#include <stdio.h>

void EmployeePayRecordsRow_Register(StatementExecutor*);


typedef struct EmployeePayRecordsRow
{
    char result[32];
} EmployeePayRecordsRow;

void* EmployeePayRecordsRow_Create(StatementExecutor* executor, SlimList* args)
{
  EmployeePayRecordsRow* self = malloc(sizeof(EmployeePayRecordsRow));
  memset(self, 0, sizeof(EmployeePayRecordsRow));
  return self;
}

void EmployeePayRecordsRow_Destroy(void* self)
{
    free(self);
}

static char* query(void* void_self, SlimList* args) {
  EmployeePayRecordsRow* self = (EmployeePayRecordsRow*)void_self;
  
  SlimList* id = SlimList_Create();
  SlimList_AddString(id, "id");
  SlimList_AddString(id, "1");

  SlimList* pay = SlimList_Create();
  SlimList_AddString(pay, "pay");
  SlimList_AddString(pay, "1000");
  
  SlimList* record1 = SlimList_Create();
  SlimList_AddList(record1, id);
  SlimList_AddList(record1, pay);
  
  SlimList* records = SlimList_Create();
  SlimList_AddList(records, record1);
  
  return SlimList_Serialize(records);
}


void EmployeePayRecordsRow_Register(StatementExecutor* executor)
{
  StatementExecutor_RegisterFixture(executor, "EmployeePayRecordsRow", EmployeePayRecordsRow_Create, EmployeePayRecordsRow_Destroy);
  StatementExecutor_RegisterMethod(executor, "TestSlim", "query", query); 
}