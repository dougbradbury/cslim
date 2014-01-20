#import "OCSReturnValue.h"

@implementation OCSReturnValue

+(NSString*) forInvocation:(NSInvocation*) invocation {
    if ([self signatureHasReturnTypeVoid: invocation.methodSignature]) {
        return @"OK";
    } else {
        id result;
        [invocation getReturnValue: &result];
        return [OCSReturnValue forObjectOrPrimitive: result
                                 andMethodSignature: invocation.methodSignature];
    }
}

+(NSString*) forObjectOrPrimitive:(id) result andMethodSignature:(NSMethodSignature*) signature {
    NSString* returnType = [NSString stringWithUTF8String: [signature methodReturnType]];
    if ([returnType isEqualToString: @"@"]) {
        if([NSStringFromClass([result class]) isEqualToString: @"NSCFString"]) {
            return result;
        } else if([NSStringFromClass([result class]) isEqualToString: @"__NSCFString"]) {
            return result;
        } else if([NSStringFromClass([result class]) isEqualToString: @"__NSCFConstantString"]) {
            return result;
        } else {
            return [result stringValue];
        }
    } else if ([returnType isEqualToString: @"i"]) {
        return [NSString stringWithFormat: @"%i", (int)result];
    } else if ([returnType isEqualToString: @"c"]) {
        if ((BOOL)result) {
            return @"true";
        } else {
            return @"false";
        }
    } else {
        return @"OK";
    }
}

+(BOOL) signatureHasReturnTypeVoid:(NSMethodSignature*) methodSignature {
    return [[NSString stringWithUTF8String: [methodSignature methodReturnType]] isEqualToString: @"v"];
}

@end
