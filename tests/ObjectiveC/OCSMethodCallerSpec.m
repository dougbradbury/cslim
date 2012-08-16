#import <SpecKit/SpecKit.h>
#import "OCSMethodCaller.h"
#import "TestSlim.h"

SpecKitContext(OCSMethodCallerSpec) {
    
    Describe(@"multiple parameters", ^{

        __block TestSlim* fixture;
        __block OCSMethodCaller* caller;

        BeforeEach(^{
            fixture = [TestSlim new];
        });

        It(@"calls a method taking two arguments", ^{
            NSArray* args = [NSArray arrayWithObjects: @"first", @"second", nil];
            caller = [OCSMethodCaller withInstance: fixture
                                      instanceName: NULL
                                        methodName: @"multiple,strings"
                                           andArgs: args];

            [caller call];

            [ExpectObj(fixture.calledWithFirstStringArg) toBeEqualTo: @"first"];
            [ExpectObj(fixture.calledWithSecondStringArg) toBeEqualTo: @"second"];
        });
        
        It(@"returns OK for a method taking two arguments", ^{
            NSArray* args = [NSArray arrayWithObjects: @"first", @"second", nil];
            caller = [OCSMethodCaller withInstance: fixture
                                      instanceName: NULL
                                        methodName: @"multiple,strings"
                                           andArgs: args];
            
            [ExpectObj([caller call]) toBeEqualTo: @"OK"];
        });

        It(@"calls a method taking three arguments", ^{
            NSArray* args = [NSArray arrayWithObjects: @"first", @"second", @"third", nil];
            caller = [OCSMethodCaller withInstance: fixture
                                      instanceName: NULL
                                        methodName: @"three,strings,method"
                                           andArgs: args];
            
            [caller call];
            
            [ExpectObj(fixture.calledWithFirstStringArg) toBeEqualTo: @"first"];
            [ExpectObj(fixture.calledWithSecondStringArg) toBeEqualTo: @"second"];
            [ExpectObj(fixture.calledWithThirdStringArg) toBeEqualTo: @"third"];
        });

        It(@"returns OK for a method taking three arguments", ^{
            NSArray* args = [NSArray arrayWithObjects: @"first", @"second", @"third", nil];
            caller = [OCSMethodCaller withInstance: fixture
                                      instanceName: NULL
                                        methodName: @"three,strings,method"
                                           andArgs: args];
            
            [ExpectObj([caller call]) toBeEqualTo: @"OK"];
        });

        It(@"returns an error for a multi part method that does not exist", ^{
            NSArray* args = [NSArray arrayWithObjects: @"first", @"second", @"third", nil];
            caller = [OCSMethodCaller withInstance: fixture
                                      instanceName: NULL
                                        methodName: @"missing,this,method"
                                           andArgs: args];

            [ExpectObj([caller call]) toBeEqualTo: @"__EXCEPTION__:message:<<NO_METHOD_IN_CLASS missing:this:method: TestSlim.>>"];
        });

    });
    
}
