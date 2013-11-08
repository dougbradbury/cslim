#import <OCDSpec2/OCDSpec2.h>
#import "OCSStatementExecutor.h"
#import "TestSlim.h"
#import "OCSException.h"

OCDSpec2Context(OCSStatementExecutorSpec) {
    
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
               
        It(@"calls a function with a string argument", ^{
            [args addObject: @"first arg"];
            [executor makeInstanceWithName: @"test_slim" className: @"TestSlim" andArgs: [NSArray array]];
            [executor callMethod: @"withStringArg" onInstanceWithName: @"test_slim" withArgs: args];

            TestSlim* test_slim_instance = (TestSlim*)[executor getInstanceWithName: @"test_slim"];
            [ExpectObj(test_slim_instance.calledWithStringArg) toBeEqualTo: @"first arg"];
        });

        It(@"returns the return value of the called method with one argument", ^{
            [args addObject: @"first arg"];
            [executor makeInstanceWithName: @"test_slim" className: @"TestSlim" andArgs: [NSArray array]];
            NSString* result = [executor callMethod: @"withStringArg" onInstanceWithName: @"test_slim" withArgs: args];
            
            [ExpectObj(result) toBeEqualTo: @"return value for one string"];
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

        It(@"returns an error when the instance is NULL", ^{
            NSString* result = [executor callMethod: NULL onInstanceWithName: @"some_instance" withArgs: NULL];

            OCSException* expectedException = [OCSException exceptionWithMessage: @"The instance some_instance. does not exist"];
            [ExpectObj(result) toBeEqualTo: [expectedException stringValue]];
        });
        
    });
    
}
