#import <OCDSpec2/OCDSpec2.h>
#import "OCSMethodCaller.h"
#import "OCSException.h"
#import "TestSlim.h"

OCDSpec2Context(OCSMethodCallerSpec) {
    
    __block TestSlim* fixture;
    __block OCSMethodCaller* caller;
    __block NSMutableArray* args;
    
    BeforeEach(^{
        fixture = [TestSlim new];
        args = [NSMutableArray array];
    });

    Describe(@"successfully calling", ^{
        
        It(@"calls a function with no args", ^{
            caller = [OCSMethodCaller withInstance: fixture
                                        methodName: @"noArgs"
                                           andArgs: args];

            [caller call];
            
            [ExpectBool(fixture.wasNoArgsCalled) toBeTrue];
        });

        It(@"calls a function with a string argument", ^{
            args = [NSArray arrayWithObjects: @"first arg", nil];
            caller = [OCSMethodCaller withInstance: fixture
                                        methodName: @"withStringArg"
                                           andArgs: args];
            
            [caller call];
            
            [ExpectObj(fixture.calledWithStringArg) toBeEqualTo: @"first arg"];
        });

        It(@"calls a function with an nsnumber argument", ^{
            args = [NSArray arrayWithObjects: @"123.45", nil];
            caller = [OCSMethodCaller withInstance: fixture
                                        methodName: @"withNSNumberArg"
                                           andArgs: args];
            
            [caller call];

            [ExpectFloat([fixture.calledWithNSNumberArg floatValue]) toBe: 123.45 withPrecision: 0.01];
        });

        It(@"calls a function with two string arguments in an array", ^{
            args = [NSArray arrayWithObjects: @"first arg", @"second arg", nil];
            caller = [OCSMethodCaller withInstance: fixture
                                        methodName: @"withMultipleArgs"
                                           andArgs: args];
            
            [caller call];
            
            [ExpectObj(fixture.calledWithFirstStringArg) toBeEqualTo: @"first arg"];
            [ExpectObj(fixture.calledWithSecondStringArg) toBeEqualTo: @"second arg"];
        });

        It(@"calls a method taking two arguments as named parameters", ^{
            args = [NSArray arrayWithObjects: @"first", @"second", nil];
            caller = [OCSMethodCaller withInstance: fixture
                                        methodName: @"multiple,strings"
                                           andArgs: args];

            [caller call];

            [ExpectObj(fixture.calledWithFirstStringArg) toBeEqualTo: @"first"];
            [ExpectObj(fixture.calledWithSecondStringArg) toBeEqualTo: @"second"];
        });
        
        It(@"calls a method taking three arguments", ^{
            args = [NSArray arrayWithObjects: @"first", @"second", @"third", nil];
            caller = [OCSMethodCaller withInstance: fixture
                                        methodName: @"three,strings,method"
                                           andArgs: args];
            
            [caller call];
            
            [ExpectObj(fixture.calledWithFirstStringArg) toBeEqualTo: @"first"];
            [ExpectObj(fixture.calledWithSecondStringArg) toBeEqualTo: @"second"];
            [ExpectObj(fixture.calledWithThirdStringArg) toBeEqualTo: @"third"];
        });

        It(@"calls a method with null if no arguments are given, but it takes one", ^{
            caller = [OCSMethodCaller withInstance: fixture
                                        methodName: @"withStringArg"
                                           andArgs: args];

            [caller call];
            
            [ExpectObj(fixture.calledWithStringArg) toBeNil];
        });

    });
    
    Describe(@"returning values", ^{
        
        It(@"returns what the method that takes no arguments returns", ^{
            caller = [OCSMethodCaller withInstance: fixture
                                        methodName: @"noArgs"
                                           andArgs: args];

            [ExpectObj([caller call]) toBeEqualTo: [fixture noArgs]];
        });

        It(@"returns the return value of the called method with one argument", ^{
            args = [NSArray arrayWithObjects: @"some arg", nil];
            caller = [OCSMethodCaller withInstance: fixture
                                        methodName: @"withStringArg"
                                           andArgs: args];

            [ExpectObj([caller call]) toBeEqualTo: @"return value for one string"];
        });

        It(@"returns the return value for a method taking two arguments as named parameters", ^{
            args = [NSArray arrayWithObjects: @"first", @"second", nil];
            caller = [OCSMethodCaller withInstance: fixture
                                        methodName: @"multiple,strings"
                                           andArgs: args];
            
            [ExpectObj([caller call]) toBeEqualTo: @"something"];
        });
        
        It(@"returns the return value of the called method with multiple arguments in an array", ^{
            args = [NSArray arrayWithObjects: @"first arg", @"second arg", nil];
            caller = [OCSMethodCaller withInstance: fixture
                                        methodName: @"withMultipleArgs"
                                           andArgs: args];
            
            [ExpectObj([caller call]) toBeEqualTo: @"return value for multiple strings"];
        });
        

        It(@"returns OK for a method taking three arguments that returns void", ^{
            args = [NSArray arrayWithObjects: @"first", @"second", @"third", nil];
            caller = [OCSMethodCaller withInstance: fixture
                                        methodName: @"three,strings,method"
                                           andArgs: args];
            
            [ExpectObj([caller call]) toBeEqualTo: @"OK"];
        });

    });

    Describe(@"handling errors", ^{
        __block OCSException* expectedException;
        
        It(@"returns an error when the method with no args does not exist", ^{
            caller = [OCSMethodCaller withInstance: fixture
                                        methodName: @"noSuchMethod"
                                           andArgs: NULL];
            
            expectedException = [OCSException exceptionWithMessage: @"NO_METHOD_IN_CLASS noSuchMethod TestSlim."];
            [ExpectObj([caller call]) toBeEqualTo: [expectedException stringValue]];
        });
        
        It(@"returns an error if the function with one argument does not exist", ^{
            args = [NSArray arrayWithObjects: @"first arg", nil];
            caller = [OCSMethodCaller withInstance: fixture
                                        methodName: @"noOtherSuchMethod"
                                           andArgs: args];
            
            expectedException = [OCSException exceptionWithMessage: @"NO_METHOD_IN_CLASS noOtherSuchMethod: TestSlim."];
            [ExpectObj([caller call]) toBeEqualTo: [expectedException stringValue]];
        });

        It(@"returns an error if calling a method with one agument, but it takes none", ^{
            args = [NSArray arrayWithObjects: @"first", nil];
            caller = [OCSMethodCaller withInstance: fixture
                                        methodName: @"noArgs"
                                           andArgs: args];
            
            expectedException = [OCSException exceptionWithMessage: @"NO_METHOD_IN_CLASS noArgs: TestSlim."];
            [ExpectObj([caller call]) toBeEqualTo: [expectedException stringValue]];
        });
        
        It(@"returns an error for a multi part method that does not exist", ^{
            args = [NSArray arrayWithObjects: @"first", @"second", @"third", nil];
            caller = [OCSMethodCaller withInstance: fixture
                                        methodName: @"missing,this,method"
                                           andArgs: args];
            
            expectedException = [OCSException exceptionWithMessage: @"NO_METHOD_IN_CLASS missing:this:method: TestSlim."];
            [ExpectObj([caller call]) toBeEqualTo: [expectedException stringValue]];
        });

        It(@"returns an error for a method taking an array that does not exist", ^{
            args  = [NSArray arrayWithObjects: @"first", @"second", @"third", nil];
            caller = [OCSMethodCaller withInstance: fixture
                                        methodName: @"missingMethod"
                                           andArgs: args];
            
            expectedException = [OCSException exceptionWithMessage: @"NO_METHOD_IN_CLASS missingMethod: TestSlim."];
            [ExpectObj([caller call]) toBeEqualTo: [expectedException stringValue]];
        });

        It(@"returns an error if the called method raises an error", ^{
            caller = [OCSMethodCaller withInstance: fixture
                                        methodName: @"raisesException"
                                           andArgs: args];
            
            expectedException = [OCSException exceptionWithMessage: @"Some exception with format"];
            [ExpectObj([caller call]) toBeEqualTo: [expectedException stringValue]];
        });

    });
}
