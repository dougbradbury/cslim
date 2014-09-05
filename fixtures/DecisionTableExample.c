#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "SlimList.h"
#include "Fixtures.h"

typedef struct Division
{
	float numerator;
	float denominator;
	char result[32];
} Division;

void* Division_Create(StatementExecutor* errorHandler, SlimList* args)
{
	Division* self = (Division*)malloc(sizeof(Division));
	memset(self, 0, sizeof(Division));
	return self;
}

void Division_Destroy(void* void_self)
{
	free(void_self);
}

static char* setNumerator(void* void_self, SlimList* args) {
	Division* self = (Division*)void_self;
	self->numerator = atof(SlimList_GetStringAt(args, 0));
	return "";
}

static char* setDenominator(void* void_self, SlimList* args) {
	Division* self = (Division*)void_self;
	self->denominator = atof(SlimList_GetStringAt(args, 0));
	if (self->denominator == 0.0)
		return SLIM_EXCEPTION("You shouldn't divide by zero now should ya?");
	return "";
}

static char* Quotient(void* void_self, SlimList* args) {
	Division* self = (Division*)void_self;
	float quotient = self->numerator / self->denominator;
	snprintf(self->result, 32, "%g", quotient);
	return self->result;
}

//These are optional.  If they aren't declared, they are ignored
static char* execute(void* void_self, SlimList* args) {
    return "";
}

static char* reset(void* void_self, SlimList* args) {
	Division* self = (Division*)void_self;
    self->denominator = 0.0f;
    self->numerator = 0.0f;
    return "";
}
static char* table(void* void_self, SlimList* args) {
    return "";
}



SLIM_CREATE_FIXTURE(Division)
	SLIM_FUNCTION(setNumerator)
	SLIM_FUNCTION(setDenominator)
	SLIM_FUNCTION(Quotient)
	SLIM_FUNCTION(execute)
	SLIM_FUNCTION(reset)
	SLIM_FUNCTION(table)
SLIM_END

