#import <Foundation/Foundation.h>

@interface TestSlim : NSObject {
    BOOL wasNoArgsCalled;
    NSString* calledWithStringArg;
    NSNumber* calledWithNSNumberArg;
    NSString* calledWithFirstStringArg;
    NSString* calledWithSecondStringArg;
}
@property (readwrite, assign) BOOL wasNoArgsCalled;
@property (nonatomic, strong) NSString* calledWithStringArg;
@property (nonatomic, strong) NSNumber* calledWithNSNumberArg;
@property (nonatomic, strong) NSString* calledWithFirstStringArg;
@property (nonatomic, strong) NSString* calledWithSecondStringArg;

-(id) initWithString: (NSString*) givenString;
-(id) initWithArray: (NSArray*) givenArray;

-(void) raisesException;

-(NSString*) noArgs;
-(NSString*) withStringArg:(NSString*) someString;
-(void) withNSNumberArg:(NSNumber*) someNSNumber;
-(NSString*) withMultipleArgs:(NSArray*) args;
-(NSString*) createTestSlimWithString: (NSString*) givenString;

-(NSNumber*) returnsNSNumber;

@end
