#import "StatementExecutor.h"
#import "AcceptanceTests-swift.h"

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
    NSAutoreleasePool* pool;
};

StatementExecutor* StatementExecutor_Create(void) {
    StatementExecutor* self = (StatementExecutor*)malloc(sizeof(StatementExecutor));
    memset(self, 0, sizeof(StatementExecutor));
    self->pool = [[NSAutoreleasePool alloc] init];
    self->instances = [[NSMutableDictionary dictionary] retain];
    self->symbols = [[NSMutableDictionary dictionary] retain];
    return self;
}

void* StatementExecutor_Instance(StatementExecutor* executor, char const* instanceName) {
    return [executor->instances valueForKey: [NSString stringWithFormat: @"%s", instanceName]];
}

char* StatementExecutor_Make(StatementExecutor* executor, char const* instanceName, char const* className, SlimList* args){
    NSString* newClassName = [NSString stringWithFormat: @"%s", className];
    NSLog(@"[SOURCE] - Invoked StatementExecutor.m Classname: %@", newClassName);
    
    for(NSString* symbol in [[executor->symbols keyEnumerator] allObjects]) {
        newClassName = [newClassName stringByReplacingOccurrencesOfString: [NSString stringWithFormat: @"$%@", symbol]
                                                               withString: [executor->symbols objectForKey: symbol]];
    }
    Class class = NSClassFromString(newClassName);
    if(class == nil) {
        [executor->instances removeObjectForKey: [NSString stringWithFormat: @"%s", instanceName]];
        return NSStringToCString([NSString stringWithFormat: @"__EXCEPTION__:message:<<NO_CLASS %@.>>", newClassName]);
    } else {
        int length = SlimList_GetLength(args);
        StatementExecutor_ReplaceSymbols(executor, args);
        @try {
            id instance;
            if(length == 0) {
                instance = [[class alloc] init];
            } else if(length == 1) {
                if([[class alloc] respondsToSelector:@selector(initWithString:)]) {
                    instance = [[class alloc] initWithString: SlimList_GetNSStringAt(args, 0)];
                }
                else {
                    instance = [[class alloc] initWithParam: SlimList_GetNSStringAt(args, 0)];
                }
            } else {
                if([[class alloc] respondsToSelector:@selector(initWithArray:)]) {
                    instance = [[class alloc] initWithArray: SlimList_ToNSArray(args)];
                }
                else {
                    instance = [[class alloc] initWithArrayOfParams: SlimList_ToNSArray(args)];
                }
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

char* StatementExecutor_Call(StatementExecutor* executor, char const* instanceName, char const* methodName, SlimList* args) {
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
    NSMethodSignature* signature = [instance methodSignatureForSelector: selector];
    NSString* returnType = [NSString stringWithUTF8String: [signature methodReturnType]];
    id result;
    @try {
        if(length == 0) {
            result = [instance performSelector: selector];
        } else if (length == 1) {
            result = [instance performSelector: selector withObject: SlimList_GetNSStringAt(args, 0)];
        } else {
            result = [instance performSelector: selector withObject: SlimList_ToNSArray(args)];
        }
        if ([returnType isEqualToString: @"@"]) {
            if([NSStringFromClass([result class]) isEqualToString: @"__NSCFString"]) {
                return NSStringToCString(result);
            } if([NSStringFromClass([result class]) isEqualToString: @"NSTaggedPointerString"]) {
                return NSStringToCString(result);
            } else {
                return NSStringToCString([result stringValue]);
            }
        } else if ([returnType isEqualToString: @"c"] || [returnType isEqualToString: @"B"]) {
            return NSStringToCString([NSString stringWithFormat: @"%s", result ? "true" : "false"]);
        } else if ([returnType isEqualToString: @"i"]) {
            return NSStringToCString([NSString stringWithFormat: @"%@", result]);
        } else {
            return "true";
        }
    } @catch (NSException* e) {
        return NSStringToCString([NSString stringWithFormat: @"__EXCEPTION__:message:<<%@ %@>>", [e name], [e reason]]);
    }
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
    [self->pool drain];
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