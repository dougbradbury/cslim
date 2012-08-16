#import <SpecKit/SpecKit.h>
#import "OCSStatementExecutor.h"
#import "TestSlim.h"

SpecKitContext(OCSStatementExecutorSpec) {
    
    Describe(@"+sharedExecutor", ^{
        
        It(@"returns an executor", ^{
            [ExpectObj([OCSStatementExecutor sharedExecutor]) toExist];
        });
        
        It(@"returns the same executor", ^{
            [ExpectObj([OCSStatementExecutor sharedExecutor]) toBeEqualTo: [OCSStatementExecutor sharedExecutor]];
        });
        
    });
    
    Describe(@"make", ^{
        
        __block OCSStatementExecutor* executor;
        __block NSMutableArray* args;
        
        BeforeEach(^{
            executor = [OCSStatementExecutor new];
            args = [NSMutableArray array];
        });
        
        It(@"makes an instance of a class with no arguments", ^{
            [executor makeInstanceWithName: @"test_slim" className: @"TestSlim" andArgs: args];
            id test_slim_instance = [executor getInstanceWithName: @"test_slim"];
            
            bool isTestSlimClass = [test_slim_instance isKindOfClass: [TestSlim class]];
            [ExpectBool(isTestSlimClass) toBeTrue];
        });
        
        It(@"makes an instance of a class with 1 argument", ^{
            [args addObject: @"starting param"];
            [executor makeInstanceWithName: @"test_slim" className: @"TestSlim" andArgs: args];
            TestSlim* test_slim_instance = (TestSlim*)[executor getInstanceWithName: @"test_slim"];
            
            [ExpectObj(test_slim_instance.calledWithStringArg) toBeEqualTo: @"starting param"];
        });
        
        It(@"makes an instance of a class with 2 arguments", ^{
            [args addObject: @"first param"];
            [args addObject: @"second param"];
            [executor makeInstanceWithName: @"test_slim" className: @"TestSlim" andArgs: args];
            TestSlim* test_slim_instance = (TestSlim*)[executor getInstanceWithName: @"test_slim"];
            
            [ExpectObj(test_slim_instance.calledWithFirstStringArg) toBeEqualTo: @"first param"];
            [ExpectObj(test_slim_instance.calledWithSecondStringArg) toBeEqualTo: @"second param"];
        });
        
        It(@"makes an instance of a class with a symbol in a class name", ^{
            [executor setSymbol: @"part" toValue: @"Test"];
            [executor makeInstanceWithName: @"test_slim" className: @"$partSlim" andArgs: args];
            TestSlim* test_slim_instance = (TestSlim*)[executor getInstanceWithName: @"test_slim"];
            
            bool isTestSlimClass = [test_slim_instance isKindOfClass: [TestSlim class]];
            [ExpectBool(isTestSlimClass) toBeTrue];
        });
        
        It(@"returns OK if the instance is successfully created", ^{
            NSString* result = [executor makeInstanceWithName: @"test_slim" className: @"TestSlim" andArgs: args];
            
            [ExpectObj(result) toBeEqualTo: @"OK"];
        });
        
        It(@"returns an error if the wrong number of arguments is passed for make (1 for 0)", ^{
            [args addObject: @"starting param"];
            
            NSString* result = [executor makeInstanceWithName: @"test_slim" className: @"TestSlimTwo" andArgs: args];
            
            [ExpectObj(result) toBeEqualTo: @"__EXCEPTION__:message:<<COULD_NOT_INVOKE_CONSTRUCTOR TestSlimTwo[1].>>"];
        });
        
        It(@"makes a different instance of the same class", ^{
            [executor makeInstanceWithName: @"test_slim" className: @"TestSlim" andArgs: args];
            [executor makeInstanceWithName: @"test_slim_2" className: @"TestSlim" andArgs: args];
            TestSlim* test_slim_instance = (TestSlim*)[executor getInstanceWithName: @"test_slim"];
            TestSlim* test_slim_instance_2 = (TestSlim*)[executor getInstanceWithName: @"test_slim_2"];
            
            [executor callMethod: @"noArgs" onInstanceWithName: @"test_slim" withArgs: args];
            
            [ExpectBool(test_slim_instance.wasNoArgsCalled) toBeTrue];
            [ExpectBool(test_slim_instance_2.wasNoArgsCalled) toBeFalse];
        });
        
        
        It(@"removes an instance when failing to create one of a different name", ^{
            [executor makeInstanceWithName: @"test_slim" className: @"TestSlim" andArgs: args];
            [executor makeInstanceWithName: @"test_slim" className: @"NoSuchClass" andArgs: args];
            
            id test_slim_instance = [executor getInstanceWithName: @"test_slim"];
            
            [ExpectObj(test_slim_instance) toBeNil];
        });
        
        It(@"returns an error if the class is missing", ^{
            NSString* result = [executor makeInstanceWithName: @"test_slim" className: @"NoSuchClass" andArgs: args];

            [ExpectObj(result) toBeEqualTo: @"__EXCEPTION__:message:<<NO_CLASS NoSuchClass.>>"];
        });

        It(@"creates an instance with one arg while replacing a symbol", ^{
            [args addObject: @"hello $person"];
            [executor setSymbol: @"person" toValue: @"bob"];
            [executor makeInstanceWithName: @"test_slim" className: @"TestSlim" andArgs: args];

            TestSlim* test_slim_instance = (TestSlim*)[executor getInstanceWithName: @"test_slim"];
            [ExpectObj(test_slim_instance.calledWithStringArg) toBeEqualTo: @"hello bob"];
        });
        
    });
    
    Describe(@"call", ^{
        
        __block OCSStatementExecutor* executor;
        __block NSMutableArray* args;
        
        BeforeEach(^{
            executor = [OCSStatementExecutor new];
            args = [NSMutableArray array];
        });
        
        It(@"returns an error if the instance does not exists", ^{
            NSString* result = [executor callMethod: @"anyMethod" onInstanceWithName: @"missing_instance" withArgs: args];
            
            [ExpectObj(result) toBeEqualTo: @"__EXCEPTION__:message:<<The instance missing_instance. does not exist>>"];
        });
        
        It(@"returns an error if the called method raises an error", ^{
            [executor makeInstanceWithName: @"test_slim" className: @"TestSlim" andArgs: args];
            NSString* result = [executor callMethod: @"raisesException" onInstanceWithName: @"test_slim" withArgs: args];
            
            [ExpectObj(result) toBeEqualTo: @"__EXCEPTION__:message:<<Some exception with format>>"];
        });
        
        It(@"calls a function with no args", ^{
            [executor makeInstanceWithName: @"test_slim" className: @"TestSlim" andArgs: args];
            [executor callMethod: @"noArgs" onInstanceWithName: @"test_slim" withArgs: args];
            
            TestSlim* test_slim_instance = (TestSlim*)[executor getInstanceWithName: @"test_slim"];
            [ExpectBool(test_slim_instance.wasNoArgsCalled) toBeTrue];
        });
        
        It(@"returns what the method that takes no arguments returns", ^{
            [executor makeInstanceWithName: @"test_slim" className: @"TestSlim" andArgs: args];
            NSString* result = [executor callMethod: @"noArgs" onInstanceWithName: @"test_slim" withArgs: args];
            
            TestSlim* test_slim_instance = (TestSlim*)[executor getInstanceWithName: @"test_slim"];
            [ExpectObj(result) toBeEqualTo: [test_slim_instance noArgs]];
        });
        
        It(@"returns an error if the function with no arguments does not exist", ^{
            [executor makeInstanceWithName: @"test_slim" className: @"TestSlim" andArgs: args];
            NSString* result = [executor callMethod: @"noSuchMethod" onInstanceWithName: @"test_slim" withArgs: args];
            
            [ExpectObj(result) toBeEqualTo: @"__EXCEPTION__:message:<<NO_METHOD_IN_CLASS noSuchMethod TestSlim.>>"];
        });
        
        It(@"returns an error if calling a method with one agument, but it takes none", ^{
            [args addObject: @"first"];
            [executor makeInstanceWithName: @"test_slim" className: @"TestSlim" andArgs: args];
            [executor callMethod: @"withStringArg" onInstanceWithName: @"test_slim" withArgs: args];
            
            NSString* result = [executor callMethod: @"noSuchMethod" onInstanceWithName: @"test_slim" withArgs: args];
            [ExpectObj(result) toBeEqualTo: @"__EXCEPTION__:message:<<NO_METHOD_IN_CLASS noSuchMethod: TestSlim.>>"];
        });
        
        It(@"calls a function with a string argument", ^{
            [args addObject: @"first arg"];
            [executor makeInstanceWithName: @"test_slim" className: @"TestSlim" andArgs: args];
            [executor callMethod: @"withStringArg" onInstanceWithName: @"test_slim" withArgs: args];
            
            TestSlim* test_slim_instance = (TestSlim*)[executor getInstanceWithName: @"test_slim"];
            [ExpectObj(test_slim_instance.calledWithStringArg) toBeEqualTo: @"first arg"];
        });
        
        It(@"calls a function with an nsnumber argument", ^{
            [args addObject: @"123.45"];
            [executor makeInstanceWithName: @"test_slim" className: @"TestSlim" andArgs: args];
            [executor callMethod: @"withNSNumberArg" onInstanceWithName: @"test_slim" withArgs: args];
            
            TestSlim* test_slim_instance = (TestSlim*)[executor getInstanceWithName: @"test_slim"];
            [ExpectBool(123.45 == [test_slim_instance.calledWithNSNumberArg doubleValue]) toBeTrue];
        });

        It(@"returns an error if the function with one argument does not exist", ^{
            [args addObject: @"first arg"];
            [executor makeInstanceWithName: @"test_slim" className: @"TestSlim" andArgs: args];

            NSString* result = [executor callMethod: @"noOtherSuchMethod" onInstanceWithName: @"test_slim" withArgs: args];
            
            [ExpectObj(result) toBeEqualTo: @"__EXCEPTION__:message:<<NO_METHOD_IN_CLASS noOtherSuchMethod: TestSlim.>>"];
        });

        It(@"calls a method with null if no arguments are given, but it takes one", ^{
            [executor makeInstanceWithName: @"test_slim" className: @"TestSlim" andArgs: args];
            [executor callMethod: @"withStringArg" onInstanceWithName: @"test_slim" withArgs: args];

            TestSlim* test_slim_instance = (TestSlim*)[executor getInstanceWithName: @"test_slim"];
            [ExpectBool(test_slim_instance.calledWithStringArg == NULL) toBeTrue];
        });

        It(@"calls a function with two string arguments", ^{
            [args addObject: @"first arg"];
            [args addObject: @"second arg"];
            [executor makeInstanceWithName: @"test_slim" className: @"TestSlim" andArgs: args];
            [executor callMethod: @"withMultipleArgs" onInstanceWithName: @"test_slim" withArgs: args];

            TestSlim* test_slim_instance = (TestSlim*)[executor getInstanceWithName: @"test_slim"];
            [ExpectObj(test_slim_instance.calledWithFirstStringArg) toBeEqualTo: @"first arg"];
            [ExpectObj(test_slim_instance.calledWithSecondStringArg) toBeEqualTo: @"second arg"];
        });
        
        It(@"returns the return value of the called method with one argument", ^{
            [args addObject: @"some arg"];
            [executor makeInstanceWithName: @"test_slim" className: @"TestSlim" andArgs: args];
            [executor callMethod: @"withStringArg" onInstanceWithName: @"test_slim" withArgs: args];

            NSString* result = [executor callMethod: @"withStringArg" onInstanceWithName: @"test_slim" withArgs: args];
            
            [ExpectObj(result) toBeEqualTo: @"return value for one string"];
        });
       
        It(@"returns the return value of the called method with multiple arguments", ^{
            [args addObject: @"first arg"];
            [args addObject: @"second arg"];            
            [executor makeInstanceWithName: @"test_slim" className: @"TestSlim" andArgs: [NSArray array]];
            
            NSString* result = [executor callMethod: @"withMultipleArgs" onInstanceWithName: @"test_slim" withArgs: args];
            
            [ExpectObj(result) toBeEqualTo: @"return value for multiple strings"];
        });

        It(@"replaces a symbol with it's value", ^{
            [args addObject: @"hello $v"];
            [executor setSymbol: @"v" toValue: @"bob"];
            [executor makeInstanceWithName: @"test_slim" className: @"TestSlim" andArgs: [NSArray array]];
            [executor callMethod: @"withStringArg" onInstanceWithName: @"test_slim" withArgs: args];
            
            TestSlim* test_slim_instance = (TestSlim*)[executor getInstanceWithName: @"test_slim"];
            [ExpectObj(test_slim_instance.calledWithStringArg) toBeEqualTo: @"hello bob"];
        });
        
        It(@"replaces a symbol in the middle", ^{
            [args addObject: @"hello $v person"];
            [executor setSymbol: @"v" toValue: @"eric"];
            [executor makeInstanceWithName: @"test_slim" className: @"TestSlim" andArgs: [NSArray array]];
            [executor callMethod: @"withStringArg" onInstanceWithName: @"test_slim" withArgs: args];
            
            TestSlim* test_slim_instance = (TestSlim*)[executor getInstanceWithName: @"test_slim"];
            [ExpectObj(test_slim_instance.calledWithStringArg) toBeEqualTo:@"hello eric person"];
        });

        It(@"replaces a symbol with other non-alphanumeric", ^{
            [args addObject: @"$v=why"];
            [executor setSymbol: @"v" toValue: @"jim"];
            [executor makeInstanceWithName: @"test_slim" className: @"TestSlim" andArgs: [NSArray array]];
            [executor callMethod: @"withStringArg" onInstanceWithName: @"test_slim" withArgs: args];
            
            TestSlim* test_slim_instance = (TestSlim*)[executor getInstanceWithName: @"test_slim"];
            [ExpectObj(test_slim_instance.calledWithStringArg) toBeEqualTo:@"jim=why"];
        });
        
        It(@"replaces multiple different symbols", ^{
            [args addObject: @"hi $v. Cost:  $12.32 from $e."];
            [executor setSymbol: @"v" toValue: @"bob"];
            [executor setSymbol: @"e" toValue: @"doug"];
            [executor makeInstanceWithName: @"test_slim" className: @"TestSlim" andArgs: [NSArray array]];
            [executor callMethod: @"withStringArg" onInstanceWithName: @"test_slim" withArgs: args];
            
            TestSlim* test_slim_instance = (TestSlim*)[executor getInstanceWithName: @"test_slim"];
            [ExpectObj(test_slim_instance.calledWithStringArg) toBeEqualTo:@"hi bob. Cost:  $12.32 from doug."];
        });
        
        It(@"does not replace a single dollar sign", ^{
            [args addObject: @"$"];
            [executor setSymbol: @"v" toValue: @"bob"];
            [executor makeInstanceWithName: @"test_slim" className: @"TestSlim" andArgs: [NSArray array]];
            [executor callMethod: @"withStringArg" onInstanceWithName: @"test_slim" withArgs: args];
            
            TestSlim* test_slim_instance = (TestSlim*)[executor getInstanceWithName: @"test_slim"];
            [ExpectObj(test_slim_instance.calledWithStringArg) toBeEqualTo:@"$"];
        });
        
        It(@"does not replace a dollar sign at the end of the string", ^{
            [args addObject: @"hi $v$"];
            [executor setSymbol: @"v" toValue: @"bob"];
            [executor makeInstanceWithName: @"test_slim" className: @"TestSlim" andArgs: [NSArray array]];
            [executor callMethod: @"withStringArg" onInstanceWithName: @"test_slim" withArgs: args];
            
            TestSlim* test_slim_instance = (TestSlim*)[executor getInstanceWithName: @"test_slim"];
            [ExpectObj(test_slim_instance.calledWithStringArg) toBeEqualTo:@"hi bob$"];
        });
        
    });
    
}
