#import <SpecKit/SpecKit.h>
#import "OCSInvocation.h"
#import "TestSlim.h"

SpecKitContext(OCSInvocationSpec) {
    
    __block TestSlim* instance;
    __block OCSInvocation* invocation;
    __block NSMutableArray* args;
    
    Describe(@"invoke", ^{
        
        BeforeEach(^{
            args = [NSMutableArray array];
            instance = [TestSlim new];
        });
        
        It(@"calls a method taking no arguments", ^{
            invocation = [OCSInvocation invocationWithInstance: instance
                                                    methodName: @"noArgs"
                                                  andArguments: args];
            
            [invocation invoke];
            
            [ExpectBool(instance.wasNoArgsCalled) toBeTrue];
        });

        It(@"calls a method taking one argument", ^{
            [args addObject: @"Foobar"];
            invocation = [OCSInvocation invocationWithInstance: instance
                                                    methodName: @"withStringArg"
                                                  andArguments: args];
            
            [invocation invoke];
            
            [ExpectObj(instance.calledWithStringArg) toBeEqualTo: @"Foobar"];
        });
        
        It(@"calls a method taking one array argument", ^{
            args = [NSArray arrayWithObjects: @"Foo", @"Bar", nil];
            invocation = [OCSInvocation invocationWithInstance: instance
                                                    methodName: @"withMultipleArgs"
                                                  andArguments: args];
            
            [invocation invoke];
            
            [ExpectObj(instance.calledWithFirstStringArg) toBeEqualTo: @"Foo"];
            [ExpectObj(instance.calledWithSecondStringArg) toBeEqualTo: @"Bar"];
        });
        
        It(@"calls a method taking two arguments", ^{
            args = [NSArray arrayWithObjects: @"first", @"second", nil];
            invocation = [OCSInvocation invocationWithInstance: instance
                                                    methodName: @"multiple,strings"
                                                  andArguments: args];
            
            [invocation invoke];
            
            [ExpectObj(instance.calledWithFirstStringArg) toBeEqualTo: @"first"];
            [ExpectObj(instance.calledWithSecondStringArg) toBeEqualTo: @"second"];
        });

        It(@"calls a method with three arguments", ^{
            args = [NSArray arrayWithObjects: @"first", @"second", @"third", nil];
            invocation = [OCSInvocation invocationWithInstance: instance
                                                    methodName: @"three,strings,method"
                                                  andArguments: args];
            
            [invocation invoke];
            
            [ExpectObj(instance.calledWithFirstStringArg) toBeEqualTo: @"first"];
            [ExpectObj(instance.calledWithSecondStringArg) toBeEqualTo: @"second"];
            [ExpectObj(instance.calledWithThirdStringArg) toBeEqualTo: @"third"];
        });

        It(@"returns the value retured by the method with no arguments", ^{
            invocation = [OCSInvocation invocationWithInstance: instance
                                                    methodName: @"noArgs"
                                                  andArguments: args];
            
            [ExpectObj([invocation invoke]) toBeEqualTo: @"return value"];
        });

        It(@"returns the value returned by the method with two arguments", ^{
            args = [NSArray arrayWithObjects: @"first", @"second", nil];
            invocation = [OCSInvocation invocationWithInstance: instance
                                                    methodName: @"multiple,strings"
                                                  andArguments: args];
            
            [ExpectObj([invocation invoke]) toBeEqualTo: @"something"];
        });
        
    });
    
}
