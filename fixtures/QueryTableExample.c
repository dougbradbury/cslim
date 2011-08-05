#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Fixtures.h"
#include "SlimList.h"
#include "SlimListSerializer.h"


typedef struct EmployeePayRecordsRow
{
	char * result;
} EmployeePayRecordsRow;

void* EmployeePayRecordsRow_Create(StatementExecutor* errorHandler, SlimList* args)
{
	EmployeePayRecordsRow* self = (EmployeePayRecordsRow*)malloc(sizeof(EmployeePayRecordsRow));
	memset(self, 0, sizeof(EmployeePayRecordsRow));
	return self;
}

void EmployeePayRecordsRow_Destroy(void* void_self)
{
	EmployeePayRecordsRow* self = (EmployeePayRecordsRow*)void_self;
	SlimList_Release(self->result);
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

	SlimList_Release(self->result);
	self->result = SlimList_Serialize(records);

	SlimList_Destroy(id);
	SlimList_Destroy(pay);
	SlimList_Destroy(record1);
	SlimList_Destroy(records);
	return self->result;
}

SLIM_CREATE_FIXTURE(EmployeePayRecordsRow)
	SLIM_FUNCTION(query)
SLIM_END

