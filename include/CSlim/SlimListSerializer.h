#ifndef SlimListSerializer_H
#define SlimListSerializer_H

#ifndef CPP_COMPILING
#ifdef __cplusplus
extern "C" {
#endif
#endif

#include "SlimList.h"

char* SlimList_Serialize(SlimList*);
void SlimList_Release(char *serializedResults);
int SlimList_SerializedLength(SlimList*);

#ifndef CPP_COMPILING
#ifdef __cplusplus
}
#endif
#endif

#endif
