#import "OCSReturnValue.h"

@implementation OCSReturnValue


+(NSString*)forObjectOrPrimitive:(id) result andMethodSignature:(NSMethodSignature*) signature {
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
        return [NSString stringWithFormat: @"%d", result];
    } else if ([returnType isEqualToString: @"c"]) {
        if (result) {
            return @"true";
        } else {
            return @"false";
        }
    } else {
        return @"OK";
    }
}

@end
