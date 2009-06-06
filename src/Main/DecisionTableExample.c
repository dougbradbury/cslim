#include <stdlib.h>
#include <memory.h>
#include <stdio.h>
#include "SlimList.h"
#include "Fixtures.h"

typedef struct Division
{
	float numerator;
	float denominator;
	char result[32];
} Division;

void* Division_Create(void* errorHandler, SlimList* args)
{
	Division* self = malloc(sizeof(Division));
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
	snprintf(self->result, 32, "%f", quotient);
	return self->result;
}


CREATE_FIXTURE(Division) 
	FUNCTION(setNumerator)
	FUNCTION(setDenominator)
	FUNCTION(Quotient)
END