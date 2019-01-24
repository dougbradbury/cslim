#ifndef D_SlimList_H
#define D_SlimList_H

#ifndef CPP_COMPILING
#ifdef __cplusplus
extern "C" {
#endif
#endif

typedef struct SlimList SlimList;
typedef struct Node     SlimListIterator;

SlimList* SlimList_Create(void);
void      SlimList_Destroy(SlimList*);

SlimListIterator* SlimList_CreateIterator(SlimList*);
int               SlimList_Iterator_HasItem(SlimListIterator*);
void              SlimList_Iterator_Advance(SlimListIterator**);
void              SlimList_Iterator_AdvanceBy(SlimListIterator**, int);

const char* SlimList_Iterator_GetString(SlimListIterator*);
SlimList*   SlimList_Iterator_GetList(SlimListIterator*);
void        SlimList_Iterator_Replace(SlimListIterator*, const char*);

extern void SlimList_AddString(SlimList*, char const*);
void        SlimList_AddList(SlimList* self, SlimList* element);
void        SlimList_PopHead(SlimList* self);
int         SlimList_GetLength(SlimList*);
int         SlimList_Equals(SlimList* self, SlimList* other);
SlimList*   SlimList_GetListAt(SlimList* self, int index);
const char* SlimList_GetStringAt(SlimList* self, int index);
double      SlimList_GetDoubleAt(SlimList* self, int index);
SlimList*   SlimList_GetHashAt(SlimList* self, int index);
void        SlimList_ReplaceAt(SlimList* self, int index, char const* replacementString);
void        SlimList_AddBuffer(SlimList* self, char const* buffer, int length);
SlimList*   SlimList_GetTailAt(SlimList*, int index);
const char* SlimList_ToString(SlimList*); /// Use CSlim_DestroyString to deallocate the string

#ifndef CPP_COMPILING
#ifdef __cplusplus
}
#endif
#endif

#endif
