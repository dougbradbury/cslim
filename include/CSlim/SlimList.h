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
void SlimList_AddString(SlimList*, char*);
void SlimList_AddList(SlimList* self, SlimList* element);
int SlimList_GetLength(SlimList*);
int SlimList_Equals(SlimList* self, SlimList* other);
SlimList * SlimList_GetListAt(SlimList* self, int index);
char * SlimList_GetStringAt(SlimList* self, int index);
void SlimList_ReplaceAt(SlimList* self, int index, char * replacementString);
void SlimList_AddBuffer(SlimList* self, char* buffer, int length);
SlimList* SlimList_GetTailAt(SlimList*, int);
char* SlimList_ToString(SlimList*);

#endif  // D_SlimList_H
