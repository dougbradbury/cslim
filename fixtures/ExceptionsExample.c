#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "SlimList.h"
#include "Fixtures.h"

typedef struct ExceptionsExample
{
} ExceptionsExample;

void* ExceptionsExample_Create(StatementExecutor* errorHandler, SlimList* args)
{
	if (SlimList_GetLength(args) < 1) {
		SLIM_CONSTRUCTOR_ERROR(errorHandler, "One arg required");
		return NULL;
	}

	ExceptionsExample* self = (ExceptionsExample*)malloc(sizeof(ExceptionsExample));
	memset(self, 0, sizeof(ExceptionsExample));
	return self;
}

void ExceptionsExample_Destroy(void* void_self)
{
	free(void_self);
}

static char* setTrouble(void* void_self, SlimList* args) {
	return SLIM_EXCEPTION("You stink");
}

SLIM_CREATE_FIXTURE(ExceptionsExample) 
	SLIM_FUNCTION(setTrouble)
SLIM_END

