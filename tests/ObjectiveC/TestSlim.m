#import "TestSlim.h"

@implementation TestSlim

@synthesize wasNoArgsCalled, calledWithStringArg, calledWithNSNumberArg;
@synthesize calledWithFirstStringArg, calledWithSecondStringArg;

-(id) initWithString: (NSString*) givenString {
    if ((self = [super init])) {
        self.calledWithStringArg = givenString;
    }
    return self;
}
-(id) initWithArray: (NSArray*) givenArray {
    if ((self = [super init])) {
        self.calledWithFirstStringArg = [givenArray objectAtIndex: 0];
        self.calledWithSecondStringArg = [givenArray objectAtIndex: 1];
    }
    return self;
}

-(void) raisesException {
    [NSException raise: @"Some exception" format: @"with format"];
}

-(NSString*) noArgs {
    self.wasNoArgsCalled = YES;
    return @"return value";
}

-(void) withVoidReturnType {
    //Do nothing
}
-(int) withIntPassThrough:(NSString*) givenString {
    return [givenString intValue];
}

-(void) setString: (NSString*) string {
    self.calledWithStringArg = string;
}
-(NSString*) getStringArg {
    return self.calledWithStringArg;
}

-(NSString*) echoBoolean: (NSString*) string {
    return string;
}
-(NSString*) withStringArg:(NSString*) someString {
    self.calledWithStringArg = someString;
    return @"return value for one string";
}

-(void) withNSNumberArg:(NSNumber*) someNSNumber {
    self.calledWithNSNumberArg = someNSNumber;
}

-(NSString*) withMultipleArgs:(NSArray*) args {
    self.calledWithFirstStringArg = [args objectAtIndex: 0];
    self.calledWithSecondStringArg = [args objectAtIndex: 1];
    return @"return value for multiple strings";
}

-(NSString*) multiple:(NSString*) firstArg strings:(NSString*) secondArg {
    self.calledWithFirstStringArg = firstArg;
    self.calledWithSecondStringArg = secondArg;
    return @"something";
}

-(void) three:(NSString*) firstArg strings:(NSString*) secondArg method:(NSString*) thirdArg {
    self.calledWithFirstStringArg = firstArg;
    self.calledWithSecondStringArg = secondArg;
    self.calledWithThirdStringArg = thirdArg;
}

-(NSString*) createTestSlimWithString: (NSString*) givenString {
    return givenString;
}

-(NSString*) getFixture {
    return @"foo";
}

-(NSNumber*) returnsNSNumber {
    return [NSNumber numberWithInt: 123];
}

@end
