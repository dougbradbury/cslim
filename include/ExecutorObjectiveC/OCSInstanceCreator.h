#import <Foundation/Foundation.h>

@interface OCSInstanceCreator : NSObject

+(id) instanceWithName:(NSString*) givenInstanceName
             ClassName:(NSString*) givenClassName
               andArgs:(NSArray*) givenArgs;

-(id) create;

-(NSString*) result;

@end
