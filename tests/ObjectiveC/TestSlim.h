#import <Foundation/Foundation.h>

@interface TestSlim : NSObject {
    BOOL wasNoArgsCalled;
    NSString* calledWithStringArg;
}
@property (assign) BOOL wasNoArgsCalled;
@property (nonatomic, retain) NSString* calledWithStringArg;
@property (nonatomic, retain) NSString* calledWithFirstStringArg;
@property (nonatomic, retain) NSString* calledWithSecondStringArg;

-(id) initWithString: (NSString*) givenString;
-(id) initWithArray: (NSArray*) givenArray;

-(NSString*) noArgs;
-(NSString*) withStringArg:(NSString*) someString;
-(NSString*) withMultipleArgs:(NSArray*) args;
-(NSString*) createTestSlimWithString: (NSString*) givenString;

@end
