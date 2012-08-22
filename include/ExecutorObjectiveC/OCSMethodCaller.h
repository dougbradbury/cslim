#import <Foundation/Foundation.h>

@interface OCSMethodCaller : NSObject

+(id) withInstance:(id) instance
      instanceName:(NSString*) instanceName
        methodName:(NSString*) methodName
           andArgs:(NSArray*) args;

-(NSString*) call;

@end
