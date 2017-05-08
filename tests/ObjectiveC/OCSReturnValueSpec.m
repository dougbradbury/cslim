#import <OCDSpec2/OCDSpec2.h>
#import "OCSReturnValue.h"
#import "ValidReturnTypes.h"
#import "SlimList.h"
#import "SlimListSerializer.h"
//#import "MacSpecs-Swift.h"

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

OCDSpec2Context(OCSReturnValueSpec) {
    
    Describe(@"forInvocation", ^{
        
        __block NSString* result;
        
        It(@"returns the result for methodReturningNSString", ^{
            result = [OCSReturnValue forInvocation: invocationForMethodNamed(@"methodReturningNSString")];
            
            [ExpectObj(classNameFor(@"methodReturningNSString")) toBeEqualTo: @"__NSCFConstantString"];
            [ExpectObj(result) toBeEqualTo: @"Hello World"];
        });
  
        It(@"returns the result for class NSTaggedPointerString", ^{
            result = [OCSReturnValue forInvocation: invocationForMethodNamed(@"methodReturning__NSTaggedPointerString")];
            [ExpectObj(classNameFor(@"methodReturning__NSTaggedPointerString")) toBeEqualTo: @"NSTaggedPointerString"];
            [ExpectObj(result) toBeEqualTo: @"1234.5"];
        });
        
        It(@"returns the result for class __NSCFString", ^{
            result = [OCSReturnValue forInvocation: invocationForMethodNamed(@"methodReturning__NSCFString")];
            [ExpectObj(classNameFor(@"methodReturning__NSCFString")) toBeEqualTo: @"__NSCFString"];
            [ExpectObj(result) toBeEqualTo: @"Hello World"];
        });
        
        It(@"returns the result for class __NSCFConstantString", ^{
            result = [OCSReturnValue forInvocation: invocationForMethodNamed(@"methodReturning__NSCFConstantString")];
            
            [ExpectObj(classNameFor(@"methodReturning__NSCFConstantString")) toBeEqualTo: @"__NSCFConstantString"];
            [ExpectObj(result) toBeEqualTo: @"Foobar"];
        });
        
        It(@"returns the result for a native Swift string class (_NSContigousString)", ^{
            result = [OCSReturnValue forInvocation: invocationForMethodNamed(@"methodReturningSwiftString")];
            
            
            [ExpectStr(classNameFor(@"methodReturningSwiftString")) toContain:@"_NSContiguousString"];
            [ExpectObj(result) toBeEqualTo: @"Hello Swift"];
        });
        
        
        It(@"returns the value of an NSNumber", ^{
            result = [OCSReturnValue forInvocation: invocationForMethodNamed( @"methodReturningNSNumber")];
            
            [ExpectObj(result) toBeEqualTo: @"123"];
        });
        
        It(@"returns the value of an int", ^{
            result = [OCSReturnValue forInvocation: invocationForMethodNamed(@"methodReturningInt")];
            
            [ExpectObj(result) toBeEqualTo: @"456"];
        });
        
        It(@"returns the value of a float", ^{
            result = [OCSReturnValue forInvocation: invocationForMethodNamed(@"methodReturningFloat")];
            
            [ExpectObj(result) toBeEqualTo: @"3.14159"];
        });
        
        It(@"returns the value of a double", ^{
            result = [OCSReturnValue forInvocation: invocationForMethodNamed(@"methodReturningDouble")];
            
            [ExpectObj(result) toBeEqualTo: @"123.45"];
        });
        
        It(@"returns the value of a Swift Double", ^{
            result = [OCSReturnValue forInvocation: invocationForMethodNamed(@"methodReturningSwiftDouble")];
            
            [ExpectObj(result) toBeEqualTo: @"123.45"];
        });
        
        It(@"returns the value of a Swift Float", ^{
            result = [OCSReturnValue forInvocation: invocationForMethodNamed(@"methodReturningSwiftFloat")];
            
            [ExpectObj(result) toBeEqualTo: @"3.14159"];
        });

        It(@"returns the value of a Swift Int", ^{
            result = [OCSReturnValue forInvocation: invocationForMethodNamed(@"methodReturningSwiftInt")];
            
            [ExpectObj(result) toBeEqualTo: @"789"];
        });
        
        It(@"returns the value of YES BOOL", ^{
            result = [OCSReturnValue forInvocation: invocationForMethodNamed(@"methodReturningBOOLYES")];
            
            [ExpectObj(result) toBeEqualTo: @"true"];
        });
        
        It(@"returns the value of NO BOOL", ^{
            result = [OCSReturnValue forInvocation: invocationForMethodNamed(@"methodReturningBOOLNO")];
            
            [ExpectObj(result) toBeEqualTo: @"false"];
        });
        
        It(@"returns the value of false Bool", ^{
            result = [OCSReturnValue forInvocation: invocationForMethodNamed(@"methodReturningBoolFalse")];
            
            [ExpectObj(result) toBeEqualTo: @"false"];
        });
        
        It(@"returns the value of true Bool", ^{
            result = [OCSReturnValue forInvocation: invocationForMethodNamed(@"methodReturningBoolTrue")];
            
            [ExpectObj(result) toBeEqualTo: @"true"];
        });
        
        It(@"returns the value of Swift true Bool", ^{
            result = [OCSReturnValue forInvocation: invocationForMethodNamed(@"methodReturningSwiftBoolTrue")];
            
            [ExpectObj(result) toBeEqualTo: @"true"];
        });
        
        It(@"returns the value of Swift false Bool", ^{
            result = [OCSReturnValue forInvocation: invocationForMethodNamed(@"methodReturningSwiftBoolFalse")];
            
            [ExpectObj(result) toBeEqualTo: @"false"];
        });
        
        It(@"returns OK for void", ^{
            result = [OCSReturnValue forInvocation: invocationForMethodNamed(@"methodReturningVoid")];
            
            [ExpectObj(result) toBeEqualTo: @"OK"];
        });

        It(@"returns Serialized Slim list for a one element NSArray", ^{
            result = [OCSReturnValue forInvocation: invocationForMethodNamed(@"methodReturningOneElementArray")];
            SlimList* list = SlimList_Create();
            SlimList_AddString(list, "123");
            NSString *resultString = [NSString stringWithUTF8String: SlimList_Serialize(list)];
            SlimList_Destroy(list);
            [ExpectObj(result) toBeEqualTo:resultString];
        });


        It(@"returns Serialized Slim list for a two element NSArray", ^{
            result = [OCSReturnValue forInvocation: invocationForMethodNamed(@"methodReturningTwoElementArray")];
            SlimList* list = SlimList_Create();
            SlimList_AddString(list, "123");
            SlimList_AddString(list, "456");
            NSString *resultString = [NSString stringWithUTF8String: SlimList_Serialize(list)];
            SlimList_Destroy(list);
            [ExpectObj(result) toBeEqualTo:resultString];
        });
        
        It(@"returns Serialized Slim list for an NSArray representing query table results", ^{
            result = [OCSReturnValue forInvocation: invocationForMethodNamed(@"methodReturningQueryTableResults")];
            SlimList* list = SlimList_Create();
            SlimList* firstDictionary = SlimList_Create();
            SlimList* secondDictionary = SlimList_Create();
            SlimList* firstDictionaryKeyValuePair = SlimList_Create();
            SlimList_AddString(firstDictionaryKeyValuePair, "key1");
            SlimList_AddString(firstDictionaryKeyValuePair, "value1");
            SlimList_AddList(firstDictionary, firstDictionaryKeyValuePair);
            SlimList_AddList(list, firstDictionary);
            SlimList* secondDictionaryKeyValuePair = SlimList_Create();
            SlimList_AddString(secondDictionaryKeyValuePair, "key2");
            SlimList_AddString(secondDictionaryKeyValuePair, "value2");
            SlimList_AddList(secondDictionary, secondDictionaryKeyValuePair);
            SlimList_AddList(list, secondDictionary);
            NSString *resultString = [NSString stringWithUTF8String: SlimList_Serialize(list)];
            SlimList_Destroy(list);
            [ExpectObj(result) toBeEqualTo:resultString];
        });
        
        It(@"returns true for @YES", ^{
            
            result = [OCSReturnValue forInvocation: invocationForMethodNamed(@"methodReturning__NSCFBooleanYES")];
            
            [ExpectObj(result) toBeEqualTo: @"true"];
        });
        
        It(@"returns true for @NO", ^{
            
            result = [OCSReturnValue forInvocation: invocationForMethodNamed(@"methodReturning__NSCFBooleanNO")];
            
            [ExpectObj(result) toBeEqualTo: @"false"];
        });
        
        It(@"returns Slim list for NSDictionaries",^{
            result = [OCSReturnValue forInvocation: invocationForMethodNamed(@"methodReturning__NSCFBooleanNO")];
            
            [ExpectObj(result) toBeEqualTo: @"false"];
        });
    });
}
