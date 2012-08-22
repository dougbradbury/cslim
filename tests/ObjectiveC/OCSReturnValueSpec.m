#import <SpecKit/SpecKit.h>
#import "OCSReturnValue.h"
#import "ValidReturnTypes.h"

NSMethodSignature* forSelector(SEL selector) {
    return [[[ValidReturnTypes alloc] init] methodSignatureForSelector: selector];
}

NSInvocation* invocationForMethodNamed(NSString* methodName) {
    ValidReturnTypes* fixture = [ValidReturnTypes new];
    NSInvocation* invocation = [NSInvocation invocationWithMethodSignature: forSelector(NSSelectorFromString(methodName))];
    [invocation setTarget: fixture];
    [invocation setSelector: NSSelectorFromString(methodName)];
    [invocation invoke];
    return invocation;
}

NSString* classNameFor(NSString* methodName) {
    return NSStringFromClass([[[ValidReturnTypes new] performSelector: NSSelectorFromString(methodName)] class]);
}
SpecKitContext(OCSReturnValueSpec) {
    
    Describe(@"forInvocation", ^{
        
        __block NSString* result;
        
        It(@"returns the result for methodReturningNSString", ^{
            result = [OCSReturnValue forInvocation: invocationForMethodNamed(@"methodReturningNSString")];
            
            [ExpectObj(classNameFor(@"methodReturningNSString")) toBeEqualTo: @"__NSCFConstantString"];
            [ExpectObj(result) toBeEqualTo: @"Hello World"];
        });
        
        It(@"returns the result for class __NSCFString", ^{
            result = [OCSReturnValue forInvocation: invocationForMethodNamed(@"methodReturning__NSCFString")];
            
            [ExpectObj(classNameFor(@"methodReturning__NSCFString")) toBeEqualTo: @"__NSCFString"];
            [ExpectObj(result) toBeEqualTo: @"1234.5"];
        });
        
        It(@"returns the result for class __NSCFConstantString", ^{
            result = [OCSReturnValue forInvocation: invocationForMethodNamed(@"methodReturning__NSCFConstantString")];
            
            [ExpectObj(classNameFor(@"methodReturning__NSCFConstantString")) toBeEqualTo: @"__NSCFConstantString"];
            [ExpectObj(result) toBeEqualTo: @"Foobar"];
        });
        
        It(@"returns the value of an NSNumber", ^{
            result = [OCSReturnValue forInvocation: invocationForMethodNamed( @"methodReturningNSNumber")];
            
            [ExpectObj(result) toBeEqualTo: @"123"];
        });
        
        It(@"returns the value of an int", ^{
            result = [OCSReturnValue forInvocation: invocationForMethodNamed(@"methodReturningInt")];
            
            [ExpectObj(result) toBeEqualTo: @"456"];
        });
        
        It(@"returns the value of YES BOOL", ^{
            result = [OCSReturnValue forInvocation: invocationForMethodNamed(@"methodReturningBOOLYES")];
            
            [ExpectObj(result) toBeEqualTo: @"true"];
        });
        
        It(@"returns the value of NO BOOL", ^{
            result = [OCSReturnValue forInvocation: invocationForMethodNamed(@"methodReturningBOOLNO")];
            
            [ExpectObj(result) toBeEqualTo: @"false"];
        });
        
        It(@"returns OK for void", ^{
            result = [OCSReturnValue forInvocation: invocationForMethodNamed(@"methodReturningVoid")];
            
            [ExpectObj(result) toBeEqualTo: @"OK"];
        });
        
    });
}