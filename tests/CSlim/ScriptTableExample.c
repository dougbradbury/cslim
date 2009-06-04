#include <stdlib.h>
#include <memory.h>
#include "StatementExecutor.h"
#include "SlimList.h"
#include <stdio.h>

void Count_Register(StatementExecutor*);


typedef struct Count
{
  int count;
  char result[32];
} Count;

void* Count_Create(StatementExecutor* executor, SlimList* args)
{
	Count* self = malloc(sizeof(Count));
	memset(self, 0, sizeof(Count));
	return self;
}

void Count_Destroy(void* self)
{
    free(self);
}

static char* Count_count(void* void_self, SlimList* args) {
	Count* self = (Count*)void_self;
  self->count++;
	return "OK";
}

static char* Count_counter(void* void_self, SlimList* args) {
	Count* self = (Count*)void_self;
  snprintf(self->result, 32, "%d", self->count);
	return self->result;
}


void Count_Register(StatementExecutor* executor)
{
	StatementExecutor_RegisterFixture(executor, "Count", Count_Create, Count_Destroy);
	StatementExecutor_RegisterMethod(executor, "TestSlim", "count", Count_count);	
	StatementExecutor_RegisterMethod(executor, "TestSlim", "counter", Count_counter);	
}