#import "OCSObjectiveCtoCBridge.h"
#import "SlimList.h"

char* NSStringToCString(NSString* string) {
    return (char*)[string UTF8String];
}

NSString* CStringToNSString(char const* cString) {
    return [NSString stringWithUTF8String: cString];
}

NSArray* SlimList_ToNSArray(SlimList* self) {
    int length = SlimList_GetLength(self);
    NSMutableArray* array = [NSMutableArray array];
    for(int i=0; i<length; i++) {
        [array addObject: SlimList_GetNSStringAt(self, i)];
    }
    return array;
}

NSString* SlimList_GetNSStringAt(SlimList* self, int index) {
    return CStringToNSString(SlimList_GetStringAt(self, index));
}