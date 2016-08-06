#include "SlimList.h"
#include "SlimUtil.h"
#include "SlimListSerializer.h"
#include "SlimUtil.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static char* stringForNullNode = "null";

enum {LIST_OVERHEAD=9, ELEMENT_OVERHEAD=8};

static char* GetStringWithNullAsANormalString(SlimListIterator* iterator)
{
	char* nodeString = SlimList_Iterator_GetString(iterator);

	if (nodeString == NULL)
		nodeString = stringForNullNode;

	return nodeString;
}

long FieldLength(unsigned char * nodeString)
{
        unsigned char *p;
        long fieldlength=0;
        for (p=nodeString;*p;p++)
          if (CSlim_IsCharacter(p) == 1)
           fieldlength++;
        return fieldlength;
}

int SlimList_SerializedLength(SlimList* self)
{
	int length = LIST_OVERHEAD;

	SlimListIterator* iterator = SlimList_CreateIterator(self);
	while (SlimList_Iterator_HasItem(iterator)) {
		length += strlen(GetStringWithNullAsANormalString(iterator)) + ELEMENT_OVERHEAD;

		SlimList_Iterator_Advance(&iterator);
	}

	return length;
}

char* SlimList_Serialize(SlimList* self)
{
        char* buf = (char*)malloc(SlimList_SerializedLength(self)+1);
        char* write_ptr = buf;
        int listLength = SlimList_GetLength(self);

        write_ptr += sprintf(write_ptr, "[%06d:", listLength);

        SlimListIterator* iterator = SlimList_CreateIterator(self);
        while (SlimList_Iterator_HasItem(iterator)) {
        	unsigned char * nodeString = (unsigned char *) GetStringWithNullAsANormalString(iterator);
        	write_ptr += sprintf(write_ptr, "%06ld:%s:", FieldLength(nodeString), nodeString);
        	
        	SlimList_Iterator_Advance(&iterator);
        }
        strcpy(write_ptr, "]");
        return buf;
}

void SlimList_Release(char *serializedResults)
{
  if(serializedResults)
    free(serializedResults);
}
