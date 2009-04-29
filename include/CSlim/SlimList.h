#ifndef D_SlimList_H
#define D_SlimList_H

///////////////////////////////////////////////////////////////////////////////
//
//  SlimList is responsible for ...
//
///////////////////////////////////////////////////////////////////////////////

typedef struct SlimList SlimList;

SlimList* SlimList_create(void);
void SlimList_destroy(SlimList*);
void SlimList_addString(SlimList*, char*);
void SlimList_addList(SlimList* self, SlimList* element);
int SlimList_getLength(SlimList*);
int SlimList_equals(SlimList* self, SlimList* other);
SlimList * SlimList_getListAt(SlimList* self, int index);
char * SlimList_getStringAt(SlimList* self, int index);
void SlimList_replaceAt(SlimList* self, int index, char * replacementString);
void SlimList_addBuffer(SlimList* self, char* buffer, int length);

#endif  // D_SlimList_H
