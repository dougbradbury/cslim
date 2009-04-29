#include "SlimList.h"
#include "SlimListSerializer.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

enum {LIST_OVERHEAD=9, ELEMENT_OVERHEAD=8};
int SlimList_serializedLength(SlimList* self)
{
	int length = LIST_OVERHEAD;
	int i;
	for(i = 0; i < SlimList_getLength(self); i++)
	{
		length += strlen(SlimList_getStringAt(self, i)) + ELEMENT_OVERHEAD;
	}
	return length;
}

char* SlimList_serialize(SlimList* self)
{
	char* buf = malloc(SlimList_serializedLength(self)+1);
	char* write_ptr = buf;
	int listLength = SlimList_getLength(self);
	int i;
		
	write_ptr += sprintf(write_ptr, "[%06d:", listLength);

	for(i = 0; i < listLength; i++)
	{
		char * nodeString = SlimList_getStringAt(self, i);
		write_ptr += sprintf(write_ptr, "%06ld:%s:", strlen(nodeString), nodeString);
	}
	strcpy(write_ptr, "]");
	return buf;
}
