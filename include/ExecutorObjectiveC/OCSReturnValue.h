#import <Foundation/Foundation.h>

@interface OCSReturnValue : NSObject {
}

+(NSString*) forInvocation:(NSInvocation*) invocation;
+(NSString*) forObjectOrPrimitive:(id) result andMethodSignature:(NSMethodSignature*) signature;

@end
