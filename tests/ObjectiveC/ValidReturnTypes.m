#import "ValidReturnTypes.h"
#import "RespondsToStringValue.h"

@implementation ValidReturnTypes

-(void) methodReturningVoid {
    // Does nothing
}

-(NSString*) methodReturningNSString {
    return @"Hello World";
}

-(NSString*) methodReturning__NSTaggedPointerString {
    return [NSString stringWithFormat:@"%g",1234.5];
}

-(NSString*) methodReturning__NSCFString {
    return [[NSMutableString alloc] initWithString:@"Hello World"];
}

-(NSString*) methodReturning__NSCFConstantString {
    return @"Foobar";
}
-(NSNumber*) methodReturningNSNumber {
    return [NSNumber numberWithInt: 123];
}

-(int) methodReturningInt {
    return 456;
}

-(RespondsToStringValue*) methodReturningObjectThatRespondsToStringValue {
    return [[RespondsToStringValue alloc] init];
}

-(BOOL) methodReturningBOOLYES {
    return YES;
}

-(BOOL) methodReturningBOOLNO {
    return NO;
}

-(_Bool)methodReturningBoolFalse {
    return false;
}

-(_Bool)methodReturningBoolTrue {
    return true;
}

- (NSArray *)methodReturningOneElementArray {
    return @[@"123"];
}

- (NSArray *)methodReturningTwoElementArray {
    return @[@"123", @"456"];
}

- (NSArray *)methodReturningQueryTableResults {
    return @[ @{ @"key1" : @"value1" }, @{ @"key2" : @"value2" } ];
}

- (NSArray *)methodReturningKnownResult {
    return @[ @{@"firstName" : @"Fred"} ];
}

- (NSNumber *)methodReturning__NSCFBooleanYES {
    return @YES;
}

- (NSNumber *)methodReturning__NSCFBooleanNO {
    return @NO;
}


@end
;
