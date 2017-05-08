#import "OCSReturnValue.h"
#import "OCSObjectiveCtoCBridge.h"
#import "SlimListSerializer.h"

@implementation OCSReturnValue

+(NSString*) forInvocation:(NSInvocation*) invocation {
    if ([self invocationHasReturnTypeVoid: invocation]) {
        return @"OK";
    } else if ([self invocationHasReturnTypeObject:invocation]){
        return [OCSReturnValue forReturnTypeObjectInvocation:invocation];
    } else {
        return [OCSReturnValue forReturnTypePrimitiveInvocation:invocation];
    }
}

+ (NSString *) forReturnTypeObjectInvocation:(NSInvocation *) invocation {
    id object;
    [invocation getReturnValue: &object];
    return [OCSReturnValue forObject:object];
}

+ (NSString *) forObject:(id)object {
    if([NSStringFromClass([object class]) isEqualToString: @"NSCFString"]) {
        return object;
    } else if([NSStringFromClass([object class]) isEqualToString: @"__NSCFString"]) {
        return object;
    } else if([NSStringFromClass([object class]) isEqualToString: @"__NSCFConstantString"]) {
        return object;
    } else if ([NSStringFromClass([object class]) isEqualToString:@"NSTaggedPointerString"]) {
        return object;
    } else if ([NSStringFromClass([object class]) containsString:@"_NSContiguousString"]) {
        return object;
    } else if ([NSStringFromClass([object class]) containsString:@"Array"]) {
        return [self forNSArray:object];
    } else if ([NSStringFromClass([object class]) isEqualToString:@"__NSCFBoolean"]) {
        return ((NSNumber *)object).boolValue ? @"true" : @"false";
    } else {
        return [object stringValue];
    }
}

+ (NSString *) forReturnTypePrimitiveInvocation:(NSInvocation *)invocation {
    
    NSString* returnType = [self returnTypeStringForInvocation:invocation];
    
    if ([returnType isEqualToString: @"i"] || [returnType isEqualToString:@"q"]) {
        int result;
        [invocation getReturnValue: &result];
        return [[NSNumber numberWithInt:result] stringValue];
    } else if ([returnType isEqualToString: @"d"]) {
        double result;
        [invocation getReturnValue: &result];
        return [[NSNumber numberWithDouble:result] stringValue];
    } else if ([returnType isEqualToString: @"c"] || [returnType isEqualToString:@"B"]) {
        BOOL result;
        [invocation getReturnValue: &result];
        return result ? @"true" : @"false";
    } else if ([returnType isEqualToString:@"f"]) {
        float result;
        [invocation getReturnValue: &result];
        return [[NSNumber numberWithFloat:result] stringValue];
    } else {
        return @"OK";
    }
    
}

+ (NSString *) forNSArray:(NSArray *)array {
    SlimList *slimlist = NSArray_ToSlimList(array);
    NSString *result = CStringToNSString(SlimList_Serialize(slimlist));
    return result;
}

+(BOOL) invocationHasReturnTypeObject:(NSInvocation *)invocation {
    return [[self returnTypeStringForInvocation:invocation] isEqualToString:@"@"];
}

+ (BOOL) invocationHasReturnTypeVoid:(NSInvocation *) invocation {
    return [[self returnTypeStringForInvocation: invocation] isEqualToString: @"v"];
}

+ (NSString *) returnTypeStringForInvocation:(NSInvocation *)invocation {
    return [NSString stringWithUTF8String: [invocation.methodSignature methodReturnType]];
}

@end
