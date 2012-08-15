#import <Foundation/Foundation.h>

@interface OCSMethodCaller : NSObject

@property (nonatomic, strong) id instance;
@property (nonatomic, strong) NSString* instanceName;
@property (nonatomic, strong) NSString* methodName;
@property (nonatomic, strong) NSArray* args;

+(id) withInstance:(id) instance
      instanceName:(NSString*) instanceName
        methodName:(NSString*) methodName
           andArgs:(NSArray*) updatedArgs;

-(NSString*) call;

@end
