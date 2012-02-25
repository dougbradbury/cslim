#import "OCDSpec/OCDSpec.h"
#import "OCSReturnValue.h"
#import "ValidReturnTypes.h"

NSMethodSignature* forSelector(SEL selector) {
    return [[[ValidReturnTypes alloc] init] methodSignatureForSelector: selector];
}

CONTEXT(OCSReturnValueSpec)
{
    __block NSMethodSignature* signature;
    
    describe(@"forObjectOrPrimitive",
             it(@"returns the string",
                ^{
                    signature = forSelector(@selector(methodReturningNSString));
                    NSString* result = [OCSReturnValue forObjectOrPrimitive: @"Hello World" andMethodSignature: signature];
                    
                    [expect(result) toBeEqualTo: @"Hello World"];
                }),
             it(@"returns the value of an NSNumber",
                ^{
                    signature = forSelector(@selector(methodReturningNSNumber));
                    NSString* result = [OCSReturnValue forObjectOrPrimitive: [NSNumber numberWithInt: 123] andMethodSignature: signature];
                    
                    [expect(result) toBeEqualTo: @"123"];
                }),
             it(@"returns the value of an int",
                ^{
                    signature = forSelector(@selector(methodReturningInt));
                    NSString* result = [OCSReturnValue forObjectOrPrimitive: (id)456 andMethodSignature: signature];
                    
                    [expect(result) toBeEqualTo: @"456"];
                }),
             it(@"returns the value of YES BOOL",
                ^{
                    signature = forSelector(@selector(methodReturningBOOLYES));
                    NSString* result = [OCSReturnValue forObjectOrPrimitive: (id)YES andMethodSignature: signature];
                    
                    [expect(result) toBeEqualTo: @"true"];
                }),
             it(@"returns the value of NO BOOL",
                ^{
                    signature = forSelector(@selector(methodReturningBOOLNO));
                    NSString* result = [OCSReturnValue forObjectOrPrimitive: (id)NO andMethodSignature: signature];
                    
                    [expect(result) toBeEqualTo: @"false"];
                }),
             it(@"returns OK for void",
                ^{
                    signature = forSelector(@selector(methodReturningVoid));
                    NSString* result = [OCSReturnValue forObjectOrPrimitive: nil andMethodSignature: signature];

                    [expect(result) toBeEqualTo: @"OK"];
                }),
             nil);
}