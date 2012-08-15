#import <Foundation/Foundation.h>

@interface OCSReturnValue : NSObject {
}

+(NSString*) forInvocation:(NSInvocation*) invocation andMethodSignature:(NSMethodSignature*) signature;
+(NSString*) forObjectOrPrimitive:(id) result andMethodSignature:(NSMethodSignature*) signature;

@end
