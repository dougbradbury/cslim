#include "SlimList.h"
#include "SlimListSerializer.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

enum {LIST_OVERHEAD=9, ELEMENT_OVERHEAD=8};

char* nodeStringAt(SlimList* self, int i)
{
        char * nodeString = SlimList_GetStringAt(self, i);
        if (nodeString == NULL)
                nodeString = "null";
        return nodeString;
}

int SlimList_SerializedLength(SlimList* self)
{
        int length = LIST_OVERHEAD;
        int i;
        for(i = 0; i < SlimList_GetLength(self); i++)
        {
                length += strlen(nodeStringAt(self, i)) + ELEMENT_OVERHEAD;
        }
        return length;
}

char* SlimList_Serialize(SlimList* self)
{
        char* buf = (char*)malloc(SlimList_SerializedLength(self)+1);
        char* write_ptr = buf;
        int listLength = SlimList_GetLength(self);
        int i;

        write_ptr += sprintf(write_ptr, "[%06d:", listLength);

        for(i = 0; i < listLength; i++)
        {
                char * nodeString = nodeStringAt(self, i);
                // jpr strlen returns number of bytes but we want chars
                // characters 
                unsigned char *p;
                long fieldlength=0;
                for (p=nodeString;*p;p++)
                  if ((*p < 0x80)||(*p > 0xbf))
                   fieldlength++;

                write_ptr += sprintf(write_ptr, "%06ld:%s:", fieldlength, nodeString);
        }
        strcpy(write_ptr, "]");
        return buf;
}

void SlimList_Release(char *serializedResults)
{
  if(serializedResults)
    free(serializedResults);
}
