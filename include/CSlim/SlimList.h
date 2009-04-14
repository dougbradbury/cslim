#ifndef D_SlimList_H
#define D_SlimList_H

///////////////////////////////////////////////////////////////////////////////
//
//  SlimList is responsible for ...
//
///////////////////////////////////////////////////////////////////////////////

typedef struct SlimList SlimList;

SlimList* SlimList_Create(void);
void SlimList_Destroy(SlimList*);
char* SlimList_serialize(SlimList*);
void SlimList_addString(SlimList*, char*);
void SlimList_addList(SlimList* self, SlimList* element);
int SlimList_serializedLength(SlimList*);
SlimList* SlimList_deserialize(char*);
int SlimList_getLength(SlimList*);
int SlimList_equals(SlimList* self, SlimList* other);
#endif  // D_SlimList_H
