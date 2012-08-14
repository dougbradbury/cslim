#import <SpecKit/SpecKit.h>
#import "StatementExecutor.h"
#import "TestSlim.h"

SpecKitContext(StatementExecutor)
{
    __block SlimList* empty;
    __block SlimList* args;
    __block StatementExecutor* statementExecutor;
    
    Describe(@"slim class", ^{
        
        BeforeEach(^{
            empty = SlimList_Create();
            args = SlimList_Create();
            statementExecutor = StatementExecutor_Create();
        });
        
        It(@"makes an instance of a class with no arguments", ^{
            StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", empty);
            id test_slim_instance = StatementExecutor_Instance(statementExecutor, "test_slim");
            
            bool isTestSlimClass = [test_slim_instance isKindOfClass: [TestSlim class]];
            [ExpectBool(isTestSlimClass) toBeTrue];
        });
        
        It(@"makes an instance of a class with 1 argument", ^{
            SlimList_AddString(args, "starting param");
            StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", args);
            TestSlim* test_slim_instance = (TestSlim*)StatementExecutor_Instance(statementExecutor, "test_slim");
            
            [ExpectObj(test_slim_instance.calledWithStringArg) toBeEqualTo: @"starting param"];
        });
        
        It(@"makes an instance of a class with 2 arguments", ^{
            SlimList_AddString(args, "first param");
            SlimList_AddString(args, "second param");
            StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", args);
            TestSlim* test_slim_instance = (TestSlim*)StatementExecutor_Instance(statementExecutor, "test_slim");
            
            [ExpectObj(test_slim_instance.calledWithFirstStringArg) toBeEqualTo: @"first param"];
            [ExpectObj(test_slim_instance.calledWithSecondStringArg) toBeEqualTo: @"second param"];
        });
        
        It(@"makes an instance of a class with a symbol in a class name", ^{
            StatementExecutor_SetSymbol(statementExecutor, "part", "Test");
            StatementExecutor_Make(statementExecutor, "test_slim", "$partSlim", args);
            TestSlim* test_slim_instance = (TestSlim*)StatementExecutor_Instance(statementExecutor, "test_slim");
            bool isTestSlimClass = [test_slim_instance isKindOfClass: [TestSlim class]];
            [ExpectBool(isTestSlimClass) toBeTrue];
        });
        //             It(@"returns an error if the wrong number of arguments is passed for make (1 for 0)",
        //                ^{
        //                    SlimList* args = SlimList_Create();
        //                    SlimList_AddString(args, "starting param");
        //                    StatementExecutor* statementExecutor = StatementExecutor_Create();
        //
        //                    NSString* result = [NSString stringWithFormat: @"%s", StatementExecutor_Make(statementExecutor, "test_slim", "TestSlimTwo", args)];
        //
        //                    [ExpectObj(result) toBeEqualTo: @"__EXCEPTION__:message:<<COULD_NOT_INVOKE_CONSTRUCTOR TestSlimTwo xxx.>>"];
        //                });
        
        It(@"makes a different instance of the same class", ^{
            StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", empty);
            StatementExecutor_Make(statementExecutor, "test_slim_2", "TestSlim", empty);
            TestSlim* test_slim_instance = (TestSlim*)StatementExecutor_Instance(statementExecutor, "test_slim");
            TestSlim* test_slim_instance_2 = (TestSlim*)StatementExecutor_Instance(statementExecutor, "test_slim_2");
            StatementExecutor_Call(statementExecutor, "test_slim", "noArgs", args);
            
            [ExpectBool(test_slim_instance.wasNoArgsCalled) toBeTrue];
            [ExpectBool(test_slim_instance_2.wasNoArgsCalled) toBeFalse];
        });
        
        It(@"removes an instance when failing to create one of a different name", ^{
            StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", empty);
            StatementExecutor_Make(statementExecutor, "test_slim", "NoSuchClass", empty);
            
            id test_slim_instance = StatementExecutor_Instance(statementExecutor, "test_slim");
            
            [ExpectObj(test_slim_instance) toBeNil];
        });
        
        It(@"returns an error if the instance does not exists", ^{
            NSString* result = [NSString stringWithFormat: @"%s", StatementExecutor_Call(statementExecutor, "missing_instance", "anyMethod", empty)];
            
            [ExpectObj(result) toBeEqualTo: @"__EXCEPTION__:message:<<The instance missing_instance. does not exist>>"];
            //        snprintf(errorMessage, 128, "%s", [[NSString stringWithFormat: @"__EXCEPTION__:message:<<The instance %s. does not exist>>", instanceName] UTF8String]);
        });
        
        It(@"returns an error if the called method raises an error", ^{
            StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", empty);
            
            NSString* result = [NSString stringWithFormat: @"%s", StatementExecutor_Call(statementExecutor, "test_slim", "raisesException", args)];
            
            [ExpectObj(result) toBeEqualTo: @"__EXCEPTION__:message:<<Some exception with format>>"];
        });
        
        It(@"calls a function with no args", ^{
            StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", empty);
            StatementExecutor_Call(statementExecutor, "test_slim", "noArgs", args);
            
            TestSlim* test_slim_instance = (TestSlim*)StatementExecutor_Instance(statementExecutor, "test_slim");
            [ExpectBool(test_slim_instance.wasNoArgsCalled) toBeTrue];
        });
        
        It(@"returns what the method that takes no arguments returns", ^{
            StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", empty);
            TestSlim* test_slim_instance = (TestSlim*)StatementExecutor_Instance(statementExecutor, "test_slim");
            
            NSString* result = [NSString stringWithFormat: @"%s", StatementExecutor_Call(statementExecutor, "test_slim", "noArgs", args)];
            
            [ExpectObj(result) toBeEqualTo: [test_slim_instance noArgs]];
        });
        
        It(@"returns an error if the function with no arguments does not exist", ^{
            StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", empty);
            NSString* result = [NSString stringWithFormat: @"%s", StatementExecutor_Call(statementExecutor, "test_slim", "noSuchMethod", args)];
            
            [ExpectObj(result) toBeEqualTo: @"__EXCEPTION__:message:<<NO_METHOD_IN_CLASS noSuchMethod[0] TestSlim.>>"];
        });
        
        It(@"returns an error if calling a method with one agument, but it takes none", ^{
            SlimList_AddString(args, "first");
            StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", empty);
            StatementExecutor_Call(statementExecutor, "test_slim", "withStringArg", args);
            
            NSString* result = [NSString stringWithFormat: @"%s", StatementExecutor_Call(statementExecutor, "test_slim", "noSuchMethod", args)];
            [ExpectObj(result) toBeEqualTo: @"__EXCEPTION__:message:<<NO_METHOD_IN_CLASS noSuchMethod[1] TestSlim.>>"];
        });
        
        It(@"calls a function with a string argument", ^{
            SlimList_AddString(args, "put the toilet seat down");
            StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", empty);
            StatementExecutor_Call(statementExecutor, "test_slim", "withStringArg", args);
            
            TestSlim* test_slim_instance = (TestSlim*)StatementExecutor_Instance(statementExecutor, "test_slim");
            [ExpectObj(test_slim_instance.calledWithStringArg) toBeEqualTo:@"put the toilet seat down"];
        });
        
        It(@"calls a function with an nsnumber argument", ^{
            SlimList_AddString(args, "123.45");
            StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", empty);
            StatementExecutor_Call(statementExecutor, "test_slim", "withNSNumberArg", args);
            
            TestSlim* test_slim_instance = (TestSlim*)StatementExecutor_Instance(statementExecutor, "test_slim");
            [ExpectBool(123.45 == [test_slim_instance.calledWithNSNumberArg doubleValue]) toBeTrue];
            
        });
        
        It(@"returns an error if the function with one argument does not exist", ^{
            SlimList_AddString(args, "put the toilet seat down");
            StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", empty);
            NSString* result = [NSString stringWithFormat: @"%s", StatementExecutor_Call(statementExecutor, "test_slim", "noOtherSuchMethod", args)];
            
            [ExpectObj(result) toBeEqualTo: @"__EXCEPTION__:message:<<NO_METHOD_IN_CLASS noOtherSuchMethod[1] TestSlim.>>"];
        });
        
        It(@"calls a method with null if no arguments are given, but it takes one", ^{
            StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", empty);
            StatementExecutor_Call(statementExecutor, "test_slim", "withStringArg", args);
            
            TestSlim* test_slim_instance = (TestSlim*)StatementExecutor_Instance(statementExecutor, "test_slim");
            [ExpectBool(test_slim_instance.calledWithStringArg == NULL) toBeTrue];
        });
        
        It(@"calls a function with two string arguments", ^{
            SlimList_AddString(args, "put the toilet seat down");
            SlimList_AddString(args, "every time");
            StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", empty);
            StatementExecutor_Call(statementExecutor, "test_slim", "withMultipleArgs", args);
            
            TestSlim* test_slim_instance = (TestSlim*)StatementExecutor_Instance(statementExecutor, "test_slim");
            [ExpectObj(test_slim_instance.calledWithFirstStringArg) toBeEqualTo:@"put the toilet seat down"];
            [ExpectObj(test_slim_instance.calledWithSecondStringArg) toBeEqualTo:@"every time"];
        });
        
        It(@"returns an error if the class is missing", ^{
            NSString* result = [NSString stringWithFormat: @"%s", StatementExecutor_Make(statementExecutor, "test_slim", "NoSuchClass", empty)];
            [ExpectObj(result) toBeEqualTo: @"__EXCEPTION__:message:<<NO_CLASS NoSuchClass.>>"];
        });
        
        It(@"returns the return value of the called method with one argument", ^{
            SlimList_AddString(args, "call with and return this");
            
            StatementExecutor* statementExecutor = StatementExecutor_Create();
            StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", empty);
            
            NSString* result = [NSString stringWithFormat: @"%s", StatementExecutor_Call(statementExecutor, "test_slim", "withStringArg", args)];
            [ExpectObj(result) toBeEqualTo: @"return value for one string"];
        });
        
        It(@"returns the return value of the called method with multiple arguments", ^{
            SlimList_AddString(args, "first");
            SlimList_AddString(args, "second");
            
            StatementExecutor* statementExecutor = StatementExecutor_Create();
            StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", empty);
            
            NSString* result = [NSString stringWithFormat: @"%s", StatementExecutor_Call(statementExecutor, "test_slim", "withMultipleArgs", args)];
            [ExpectObj(result) toBeEqualTo: @"return value for multiple strings"];
        });
        
        It(@"replaces a symbol with it's value", ^{
            SlimList_AddString(args, "hello $v");
            StatementExecutor_SetSymbol(statementExecutor, "v", "bob");
            StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", empty);
            StatementExecutor_Call(statementExecutor, "test_slim", "withStringArg", args);
            
            TestSlim* test_slim_instance = (TestSlim*)StatementExecutor_Instance(statementExecutor, "test_slim");
            [ExpectObj(test_slim_instance.calledWithStringArg) toBeEqualTo:@"hello bob"];
        });
        
        It(@"replaces a symbol in the middle", ^{
            SlimList_AddString(args, "hello $v person");
            StatementExecutor_SetSymbol(statementExecutor, "v", "eric");
            StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", empty);
            StatementExecutor_Call(statementExecutor, "test_slim", "withStringArg", args);
            
            TestSlim* test_slim_instance = (TestSlim*)StatementExecutor_Instance(statementExecutor, "test_slim");
            [ExpectObj(test_slim_instance.calledWithStringArg) toBeEqualTo:@"hello eric person"];
        });
        
        It(@"replaces a symbol with other non-alphanumeric", ^{
            SlimList_AddString(args, "$v=why");
            StatementExecutor_SetSymbol(statementExecutor, "v", "jim");
            StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", empty);
            StatementExecutor_Call(statementExecutor, "test_slim", "withStringArg", args);
            
            TestSlim* test_slim_instance = (TestSlim*)StatementExecutor_Instance(statementExecutor, "test_slim");
            [ExpectObj(test_slim_instance.calledWithStringArg) toBeEqualTo:@"jim=why"];
        });
        
        It(@"replaces multiple different symbols", ^{
            SlimList_AddString(args, "hi $v. Cost:  $12.32 from $e.");
            StatementExecutor_SetSymbol(statementExecutor, "v", "bob");
            StatementExecutor_SetSymbol(statementExecutor, "e", "doug");
            StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", empty);
            StatementExecutor_Call(statementExecutor, "test_slim", "withStringArg", args);
            
            TestSlim* test_slim_instance = (TestSlim*)StatementExecutor_Instance(statementExecutor, "test_slim");
            [ExpectObj(test_slim_instance.calledWithStringArg) toBeEqualTo:@"hi bob. Cost:  $12.32 from doug."];
        });
        
        It(@"does not replace a single dollar sign", ^{
            SlimList_AddString(args, "$");
            StatementExecutor_SetSymbol(statementExecutor, "v", "bob");
            StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", empty);
            StatementExecutor_Call(statementExecutor, "test_slim", "withStringArg", args);
            
            TestSlim* test_slim_instance = (TestSlim*)StatementExecutor_Instance(statementExecutor, "test_slim");
            [ExpectObj(test_slim_instance.calledWithStringArg) toBeEqualTo:@"$"];
        });
        
        It(@"does not replace a dollar sign at the end of the string", ^{
            SlimList_AddString(args, "hi $v$");
            StatementExecutor* statementExecutor = StatementExecutor_Create();
            StatementExecutor_SetSymbol(statementExecutor, "v", "bob");
            StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", empty);
            StatementExecutor_Call(statementExecutor, "test_slim", "withStringArg", args);
            
            TestSlim* test_slim_instance = (TestSlim*)StatementExecutor_Instance(statementExecutor, "test_slim");
            [ExpectObj(test_slim_instance.calledWithStringArg) toBeEqualTo:@"hi bob$"];
        });
        
        It(@"replaces a symbol in a sublist", ^{
            
        });
        
        It(@"creates an instance with one arg while replacing a symbol", ^{
            SlimList_AddString(args, "hello $person");
            StatementExecutor_SetSymbol(statementExecutor, "person", "bob");
            StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", args);
            TestSlim* test_slim_instance = (TestSlim*)StatementExecutor_Instance(statementExecutor, "test_slim");
            
            [ExpectObj(test_slim_instance.calledWithStringArg) toBeEqualTo: @"hello bob"];
        });
        
        It(@"creates an instance with one arg while replacing two symbols", ^{
            SlimList_AddString(args, "hello $person $animal");
            StatementExecutor_SetSymbol(statementExecutor, "person", "bob");
            StatementExecutor_SetSymbol(statementExecutor, "animal", "dog");
            StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", args);
            TestSlim* test_slim_instance = (TestSlim*)StatementExecutor_Instance(statementExecutor, "test_slim");
            
            [ExpectObj(test_slim_instance.calledWithStringArg) toBeEqualTo: @"hello bob dog"];
        });
        
        It(@"creates an instance with two args while replacing two symbols", ^{
            SlimList_AddString(args, "hello $person $animal");
            SlimList_AddString(args, "hello $animal");
            StatementExecutor_SetSymbol(statementExecutor, "person", "bob");
            StatementExecutor_SetSymbol(statementExecutor, "animal", "dog");
            StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", args);
            TestSlim* test_slim_instance = (TestSlim*)StatementExecutor_Instance(statementExecutor, "test_slim");
            
            [ExpectObj(test_slim_instance.calledWithFirstStringArg) toBeEqualTo: @"hello bob dog"];
            [ExpectObj(test_slim_instance.calledWithSecondStringArg) toBeEqualTo: @"hello dog"];
        });
        
        It(@"returns OK when calling a method with return type of void", ^{
            NSLog(@"HERE1");
            
            StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", args);
            NSString* result = [NSString stringWithUTF8String: StatementExecutor_Call(statementExecutor, "test_slim", "withVoidReturnType", args)];
            
            [ExpectObj(result) toBeEqualTo: @"OK"];
        });
        
        It(@"converts an int to a string when returning", ^{
            SlimList_AddString(args, "123");
            StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", args);
            NSString* result = [NSString stringWithUTF8String: StatementExecutor_Call(statementExecutor, "test_slim", "withIntPassThrough", args)];
            
            [ExpectObj(result) toBeEqualTo: @"123"];
        });
        
        It(@"converts an NSNumber to a string when returning", ^{
            StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", args);
            NSString* result = [NSString stringWithUTF8String: StatementExecutor_Call(statementExecutor, "test_slim", "returnsNSNumber", args)];
            TestSlim* test_slim_instance = (TestSlim*)StatementExecutor_Instance(statementExecutor, "test_slim");
            [ExpectObj(result) toBeEqualTo: [[test_slim_instance returnsNSNumber] stringValue]];
        });
        //             It(@"mvkdn",
        //                ^{
        //                    SlimList* args = SlimList_Create();
        //                    SlimList_AddString(args, "Köln");
        //                    StatementExecutor* statementExecutor = StatementExecutor_Create();
        //                    StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", args);
        //                    TestSlim* test_slim_instance = (TestSlim*)StatementExecutor_Instance(statementExecutor, "test_slim");
        //
        //                    [ExpectObj(test_slim_instance.calledWithStringArg) toBeEqualTo: @"Köln"];
        //                });
    });
}