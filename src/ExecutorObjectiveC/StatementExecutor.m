#import "StatementExecutor.h"


void StatementExecutor_ReplaceSymbols(StatementExecutor* executor, SlimList* args);
SEL NSSelectorFromCStringAndLength(char const* methodName, int numberOrArguments);
NSString* SlimList_GetNSStringAt(SlimList* self, int index);
char* noMethodErrorFor(char const* methodName, int length);
NSArray* SlimList_ToNSArray(SlimList* self);
char* NSStringToCString(NSString* string);

struct StatementExecutor
{
    NSMutableDictionary* instances;
    NSMutableDictionary* symbols;
};

StatementExecutor* StatementExecutor_Create(void) {
	StatementExecutor* self = (StatementExecutor*)malloc(sizeof(StatementExecutor));
	memset(self, 0, sizeof(StatementExecutor));
    self->instances = [NSMutableDictionary dictionary];
    self->symbols = [NSMutableDictionary dictionary];
	return self;
}

void* StatementExecutor_Instance(StatementExecutor* executor, char const* instanceName) {
    return [executor->instances valueForKey: [NSString stringWithFormat: @"%s", instanceName]];
}

char* StatementExecutor_Make(StatementExecutor* executor, char const* instanceName, char const* className, SlimList* args){
    Class class = NSClassFromString([NSString stringWithFormat: @"%s", className]);

//    NSString* newClassName = [NSString stringWithFormat: @"%s", className];
//    for(NSString* symbol in [[executor->symbols keyEnumerator] allObjects]) {
//        newClassName = [newClassName stringByReplacingOccurrencesOfString: [NSString stringWithFormat: @"$%@", symbol]
//                                                               withString: [executor->symbols objectForKey: symbol]];
//    }
//    Class class = NSClassFromString(newClassName);
//    NSLog(@"instanceName: %s", instanceName);
//    NSLog(@"className: %s", className);
    if(class == nil) {
        [executor->instances removeObjectForKey: [NSString stringWithFormat: @"%s", instanceName]];
//        return NSStringToCString([NSString stringWithFormat: @"__EXCEPTION__:message:<<NO_CLASS %@.>>", newClassName]);
        return NSStringToCString([NSString stringWithFormat: @"__EXCEPTION__:message:<<NO_CLASS %s.>>", className]);
    } else {
        int length = SlimList_GetLength(args);
        StatementExecutor_ReplaceSymbols(executor, args);
        @try {
            id instance;
            if(length == 0) {
                instance = [[class alloc] init];
            } else if(length == 1) {
                instance = [[class alloc] initWithString: SlimList_GetNSStringAt(args, 0)];
            } else {
                instance = [[class alloc] initWithArray: SlimList_ToNSArray(args)];
            }
            [executor->instances setValue: instance
                                   forKey: [NSString stringWithFormat: @"%s", instanceName]];
        }
        @catch (NSException* exception) {
            return "__EXCEPTION__:message:<<COULD_NOT_INVOKE_CONSTRUCTOR TestSlimTwo xxx.>>";
        }
        return "OK";
    }
}

char* StatementExecutor_Call(StatementExecutor* executor, char const* instanceName, char const* methodName, SlimList* args){
    id instance = StatementExecutor_Instance(executor, instanceName);
    int length = SlimList_GetLength(args);
    SEL selector = NSSelectorFromCStringAndLength(methodName, length);
    if(instance == NULL) {
        return NSStringToCString([NSString stringWithFormat: @"__EXCEPTION__:message:<<The instance %s. does not exist>>", instanceName]);
    }
    if(![instance respondsToSelector: selector]) {
        return noMethodErrorFor(methodName, length);
    }
    StatementExecutor_ReplaceSymbols(executor, args);
    if(length == 0) {
        return NSStringToCString([instance performSelector: selector]);
    } else if (length == 1) {
        return NSStringToCString([instance performSelector: selector withObject: SlimList_GetNSStringAt(args, 0)]);
    } else {
        return NSStringToCString([instance performSelector: selector withObject: SlimList_ToNSArray(args)]);
    }
    
    return "OK";
}

void StatementExecutor_ReplaceSymbols(StatementExecutor* executor, SlimList* args) {
    for(int i=0; i<SlimList_GetLength(args); i++) {
        NSString* newArgument = SlimList_GetNSStringAt(args, i);
        for(NSString* symbol in [[executor->symbols keyEnumerator] allObjects]) {
            newArgument = [newArgument stringByReplacingOccurrencesOfString: [NSString stringWithFormat: @"$%@", symbol]
                                                                 withString: [executor->symbols objectForKey: symbol]];
            SlimList_ReplaceAt(args, i, NSStringToCString(newArgument));
        }
    }
}

void StatementExecutor_SetSymbol(StatementExecutor* self, char const* symbol, char const* value) {
    [self->symbols setObject: [NSString stringWithUTF8String: value]
                      forKey: [NSString stringWithUTF8String: symbol]];
}


void StatementExecutor_Destroy(StatementExecutor* self) {
    [self->instances release];
    [self->symbols release];
    free(self);
}


void StatementExecutor_AddFixture(StatementExecutor* executor, Fixture fixture) {
}
void StatementExecutor_RegisterFixture(StatementExecutor* executor, char const * className, Constructor constructor, Destructor destructor){
}
void StatementExecutor_RegisterMethod(StatementExecutor* executor, char const * className, char const * methodName, Method method){
}


char* noMethodErrorFor(char const* methodName, int length) {
    return NSStringToCString([NSString stringWithFormat: @"__EXCEPTION__:message:<<NO_METHOD_IN_CLASS %s[%d] TestSlim.>>", methodName, length]);
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

NSString* SlimList_GetNSStringAt(SlimList* self, int index) {
    return [NSString stringWithFormat:@"%s", SlimList_GetStringAt(self, index)];
}