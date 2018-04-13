#include <stdlib.h>
#include <string.h>

#include "Fixtures.h"
#include "SlimList.h"

typedef struct Echo
{
    char result[32];
} Echo;

void* Echo_Create(StatementExecutor* errorHandler, SlimList* args)
{
    Echo* self = (Echo*)malloc(sizeof(Echo));
    memset(self, 0, sizeof(Echo));
    return self;
}

void Echo_Destroy(void *self)
{
    free(self);
}

static char* echo(void* void_self, SlimList* args)
{
    if (SlimList_GetLength(args) != 1)
    {
        return SLIM_EXCEPTION("Exactly 1 argument expected");
    }

    Echo* self = (Echo*)void_self;
    char* stringToEcho = SlimList_GetStringAt(args, 0);
    strncpy(self->result, stringToEcho, 32);
    self->result[31] = '\0';
    return self->result;
}

SLIM_CREATE_FIXTURE(Echo)
    SLIM_FUNCTION(echo)
SLIM_END
