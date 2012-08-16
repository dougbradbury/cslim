#import <Foundation/Foundation.h>

@interface OCSInvocation : NSObject

@property (nonatomic, strong) NSInvocation* invocation;
@property (nonatomic, strong) NSArray* args;
@property (nonatomic, strong) NSString* methodName;
@property (nonatomic, strong) id instance;

+(id) invocationWithInstance: (id) givenInstance
                  methodName: (NSString*) givenMethodName
                andArguments: (NSArray*) givenArgs;

-(id) initWithInstance: (id) givenInstance
            methodName: (NSString*) givenMethodName
          andArguments: (NSArray*) givenArgs;

-(NSString*) invoke;

+(SEL) selectorForString:(NSString*) string andArgs:(NSArray*) args;

@end