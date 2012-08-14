#import "StatementExecutor.h"
#import "OCSStatementExecutor.h"

SEL NSSelectorFromCStringAndLength(char const* methodName, int numberOrArguments);
NSString* SlimList_GetNSStringAt(SlimList* self, int index);
NSArray* SlimList_ToNSArray(SlimList* self);
char* NSStringToCString(NSString* string);
NSString* CStringToNSString(char const* cString);

struct StatementExecutor
{
};

StatementExecutor* StatementExecutor_Create(void) {
    [[OCSStatementExecutor sharedExecutor] reset];
	return nil;
}

void* StatementExecutor_Instance(StatementExecutor* executor, char const* instanceName) {
    return (__bridge void *)([[OCSStatementExecutor sharedExecutor] instanceWithName: CStringToNSString(instanceName)]);
}

char* StatementExecutor_Make(StatementExecutor* executor, char const* instanceName, char const* className, SlimList* args){
    return NSStringToCString([[OCSStatementExecutor sharedExecutor] makeInstanceWithName: CStringToNSString(instanceName)
                                                                               className: CStringToNSString(className)
                                                                                 andArgs: SlimList_ToNSArray(args)]);
}

char* StatementExecutor_Call(StatementExecutor* executor, char const* instanceName, char const* methodName, SlimList* args) {
    return NSStringToCString([[OCSStatementExecutor sharedExecutor] callMethod: CStringToNSString(methodName)
                                                            onInstanceWithName: CStringToNSString(instanceName)
                                                                      withArgs: SlimList_ToNSArray(args)]);
}

void StatementExecutor_SetSymbol(StatementExecutor* self, char const* symbol, char const* value) {
    [[OCSStatementExecutor sharedExecutor] setSymbol: CStringToNSString(symbol)
                                             toValue: CStringToNSString(value)];
}


void StatementExecutor_Destroy(StatementExecutor* self) {
    free(self);
}


void StatementExecutor_AddFixture(StatementExecutor* executor, Fixture fixture) {
}
void StatementExecutor_RegisterFixture(StatementExecutor* executor, char const * className, Constructor constructor, Destructor destructor){
}
void StatementExecutor_RegisterMethod(StatementExecutor* executor, char const * className, char const * methodName, Method method){
}

SEL NSSelectorFromCStringAndLength(char const* methodName, int numberOrArguments) {
    if (numberOrArguments == 0) {
        return NSSelectorFromString([NSString stringWithFormat:@"%s", methodName]);
    } else {
        return NSSelectorFromString([NSString stringWithFormat:@"%s:", methodName]);
    }
}

NSArray* SlimList_ToNSArray(SlimList* self) {
    int length = SlimList_GetLength(self);
    NSMutableArray* array = [NSMutableArray array];
    for(int i=0; i<length; i++) {
        [array addObject: SlimList_GetNSStringAt(self, i)];
    }
    return array;
}

char* NSStringToCString(NSString* string) {
    return (char*)[string UTF8String];
}

NSString* CStringToNSString(char const* cString) {
    return [NSString stringWithFormat: @"%s", cString];
}
NSString* SlimList_GetNSStringAt(SlimList* self, int index) {
    return [NSString stringWithFormat:@"%s", SlimList_GetStringAt(self, index)];
}