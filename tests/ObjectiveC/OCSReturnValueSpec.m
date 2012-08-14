#import <SpecKit/SpecKit.h>
#import "OCSReturnValue.h"
#import "ValidReturnTypes.h"

NSMethodSignature* forSelector(SEL selector) {
    return [[[ValidReturnTypes alloc] init] methodSignatureForSelector: selector];
}

SpecKitContext(OCSReturnValueSpec) {
    
    __block NSMethodSignature* signature;
    
    Describe(@"forObjectOrPrimitive", ^{
        
        It(@"returns the string", ^{
            signature = forSelector(@selector(methodReturningNSString));
            NSString* result = [OCSReturnValue forObjectOrPrimitive: @"Hello World"
                                                 andMethodSignature: signature];
            
            [ExpectObj(result) toBeEqualTo: @"Hello World"];
        });
        
        It(@"returns the result for class __NSCFString", ^{
            signature = forSelector(@selector(methodReturningNSString));
            
            NSString* stringWithFormat = [NSString stringWithFormat: @"%g", 1234.5];
            NSString* result = [OCSReturnValue forObjectOrPrimitive: stringWithFormat
                                                 andMethodSignature:signature];
            
            [ExpectObj(result) toBeEqualTo: @"1234.5"];
        });
        
        It(@"returns the result for class __NSCFConstantString", ^{
            signature = forSelector(@selector(methodReturningNSString));
            
            id someString = @"Foobar";
            
            NSString* result = [OCSReturnValue forObjectOrPrimitive: someString
                                                 andMethodSignature:signature];
            
            [ExpectObj(result) toBeEqualTo: @"Foobar"];
        });
        
        It(@"returns the value of an NSNumber", ^{
            signature = forSelector(@selector(methodReturningNSNumber));
            NSString* result = [OCSReturnValue forObjectOrPrimitive: [NSNumber numberWithInt: 123]
                                                 andMethodSignature: signature];
            
            [ExpectObj(result) toBeEqualTo: @"123"];
        });
        
        It(@"returns the value of an int", ^{
            signature = forSelector(@selector(methodReturningInt));
            NSString* result = [OCSReturnValue forObjectOrPrimitive: (id)456
                                                 andMethodSignature: signature];
            
            [ExpectObj(result) toBeEqualTo: @"456"];
        });
        
        It(@"returns the value of YES BOOL", ^{
            signature = forSelector(@selector(methodReturningBOOLYES));
            NSString* result = [OCSReturnValue forObjectOrPrimitive: (id)YES
                                                 andMethodSignature: signature];
            
            [ExpectObj(result) toBeEqualTo: @"true"];
        });
        
        It(@"returns the value of NO BOOL", ^{
            signature = forSelector(@selector(methodReturningBOOLNO));
            NSString* result = [OCSReturnValue forObjectOrPrimitive: (id)NO
                                                 andMethodSignature: signature];
            
            [ExpectObj(result) toBeEqualTo: @"false"];
        });
        
        It(@"returns OK for void", ^{
            signature = forSelector(@selector(methodReturningVoid));
            NSString* result = [OCSReturnValue forObjectOrPrimitive: nil
                                                 andMethodSignature: signature];
            
            [ExpectObj(result) toBeEqualTo: @"OK"];
        });
    });
    
}