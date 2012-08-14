#import <Foundation/Foundation.h>
#import "SlimList.h"

char* NSStringToCString(NSString* string);
NSString* CStringToNSString(char const* cString);

SEL NSSelectorFromCStringAndLength(char const* methodName, int numberOrArguments);
NSString* SlimList_GetNSStringAt(SlimList* self, int index);
NSArray* SlimList_ToNSArray(SlimList* self);
