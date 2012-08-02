#include "SlimList.h"
#include <stdlib.h>
#include <string.h>

#define SKIP(a) \
if (*current != (a))\
{\
        SlimList_Destroy(list); \
        return 0;\
}\
current++;

int readLength(char const** readPtr)
{
        int length = atoi(*readPtr);
        *readPtr += 6;
        return length;
}

SlimList* SlimList_Deserialize(char const* serializedList)
{
        int listLength;
        SlimList * list = 0;
        char const* current = 0;

        if(serializedList == 0 || strlen(serializedList) == 0)
                return 0;

        current = serializedList;
        list = SlimList_Create();

        SKIP('[')

        listLength = readLength(&current);

        SKIP(':')

        while (listLength--)
        {
                int elementLength = readLength(&current);
                SKIP(':')
                // JPR calculate length handling multibyte chars 
                char *p;
                int chars=0;
                int bytes=0;
                for (p=current;chars<=elementLength;p++)
                {
                  bytes++;
                  if ((*p < 0x80) || (*p > 0xBF))
                    chars++;
                }
                // chars will extend one beyond elementLength so
                // check and reduce bytes by one.
                if (chars > elementLength)
                  bytes--;
                elementLength=bytes;

                SlimList_AddBuffer(list, current, elementLength);
                current += elementLength;
                SKIP(':')
        }

        SKIP(']')
        return list;
}

