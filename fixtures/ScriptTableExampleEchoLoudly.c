#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "Fixtures.h"
#include "SlimList.h"

typedef struct EchoLoudly
{
    char result[32];
} EchoLoudly;

void* EchoLoudly_Create(StatementExecutor* errorHandler, SlimList* args)
{
    EchoLoudly* self = (EchoLoudly*)malloc(sizeof(EchoLoudly));
    memset(self, 0, sizeof(EchoLoudly));
    return self;
}

void EchoLoudly_Destroy(void* self)
{
    free(self);
}

static char* echo(void* void_self, SlimList* args)
{
    if (SlimList_GetLength(args) != 1)
    {
        return SLIM_EXCEPTION("Exactly 1 argument expected");
    }

    EchoLoudly* self = (EchoLoudly*)void_self;
    char* stringToEcho = SlimList_GetStringAt(args, 0);

    strncpy(self->result, stringToEcho, 32);
    self->result[31] = '\0';

    for (char* i = self->result; *i != '\0'; ++i)
    {
        *i = toupper(*i);
    }

    return self->result;
}

static char* repeat(void* void_self, SlimList* args)
{
    if (SlimList_GetLength(args) != 0)
    {
        return SLIM_EXCEPTION("Exactly 0 arguments expected");
    }

    EchoLoudly* self = (EchoLoudly*)void_self;
    return self->result;
}

SLIM_CREATE_FIXTURE(EchoLoudly)
    SLIM_FUNCTION(echo)
    SLIM_FUNCTION(repeat)
SLIM_END
