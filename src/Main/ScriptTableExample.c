#include <stdlib.h>
#include <memory.h>
#include <stdio.h>

#include "Fixtures.h"
#include "SlimList.h"

typedef struct Count
{
	int count;
	char result[32];
} Count;

void* Count_Create(void* errorHandler, SlimList* args)
{
	Count* self = malloc(sizeof(Count));
	memset(self, 0, sizeof(Count));
	return self;
}

void Count_Destroy(void* self)
{
	free(self);
}

static char* count(void* void_self, SlimList* args) {
	Count* self = (Count*)void_self;
	self->count++;
	return "";
}

static char* counter(void* void_self, SlimList* args) {
	Count* self = (Count*)void_self;
	snprintf(self->result, 32, "%d", self->count);
	return self->result;
}


CREATE_FIXTURE(Count)
	FUNCTION(count)
	FUNCTION(counter)	
END