#import <Foundation/Foundation.h>

@interface OCSInstance : NSObject

@property(nonatomic, strong) NSString* instanceName;
@property(nonatomic, strong) NSString* className;
@property(nonatomic, strong) NSArray* args;

@property(nonatomic, strong) NSString* result;
@property(nonatomic, strong) id instance;

+(id) instanceWithName:(NSString*) givenInstanceName
             ClassName:(NSString*) givenClassName
               andArgs:(NSArray*) givenArgs;

-(id) create;

@end
