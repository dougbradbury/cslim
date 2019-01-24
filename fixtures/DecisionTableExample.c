#include "Fixtures.h"
#include "SlimList.h"
#include "compatibility.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Division
{
  double numerator;
  double denominator;
  char   result[32];
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

static const char* setNumerator(void* void_self, SlimList* args)
{
  Division* self = (Division*)void_self;
  self->numerator = atof(SlimList_GetStringAt(args, 0));
  return "";
}

static const char* setDenominator(void* void_self, SlimList* args)
{
  Division* self = (Division*)void_self;
  self->denominator = atof(SlimList_GetStringAt(args, 0));
  if (self->denominator == 0.0)
    return SLIM_EXCEPTION("You shouldn't divide by zero now should ya?");
  return "";
}

static const char* Quotient(void* void_self, SlimList* args)
{
  Division* self = (Division*)void_self;
  double    quotient = self->numerator / self->denominator;
  snprintf(self->result, 32, "%g", quotient);
  return self->result;
}

//These are optional.  If they aren't declared, they are ignored
static const char* execute(void* void_self, SlimList* args)
{
  return "";
}

static const char* reset(void* void_self, SlimList* args)
{
  Division* self = (Division*)void_self;
  self->denominator = 0.0f;
  self->numerator = 0.0f;
  return "";
}
static const char* table(void* void_self, SlimList* args)
{
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
