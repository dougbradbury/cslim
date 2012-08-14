#import <Foundation/Foundation.h>

@interface OCSReturnValue : NSObject {
}

+(NSString*)forObjectOrPrimitive:(id) result andMethodSignature:(NSMethodSignature*) signature;

@end
