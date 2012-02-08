#import <Foundation/Foundation.h>

@interface TestSlim : NSObject {
    BOOL wasNoArgsCalled;
    NSString* calledWithStringArg;
    NSNumber* calledWithNSNumberArg;
}
@property (assign) BOOL wasNoArgsCalled;
@property (nonatomic, retain) NSString* calledWithStringArg;
@property (nonatomic, retain) NSNumber* calledWithNSNumberArg;
@property (nonatomic, retain) NSString* calledWithFirstStringArg;
@property (nonatomic, retain) NSString* calledWithSecondStringArg;

-(id) initWithString: (NSString*) givenString;
-(id) initWithArray: (NSArray*) givenArray;

-(NSString*) noArgs;
-(NSString*) withStringArg:(NSString*) someString;
-(void) withNSNumberArg:(NSNumber*) someNSNumber;
-(NSString*) withMultipleArgs:(NSArray*) args;
-(NSString*) createTestSlimWithString: (NSString*) givenString;

-(NSNumber*) returnsNSNumber;

@end
