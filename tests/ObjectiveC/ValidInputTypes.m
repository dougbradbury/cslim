#import "ValidInputTypes.h"

@implementation ValidInputTypes

@synthesize calledWith, callWithZeroArgsCalled;

-(id) init {
    if ((self = [super init])) {
        self.callWithZeroArgsCalled = NO;
    }
    return self;
}
-(void) callWithNSString: (NSString*) string {
    self.calledWith = string;
}

-(void) callWithZeroArgs {
    self.callWithZeroArgsCalled = YES;
}

@end
