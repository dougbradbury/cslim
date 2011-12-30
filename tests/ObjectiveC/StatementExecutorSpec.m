#import "OCDSpec/OCDSpec.h"
#import "StatementExecutor.h"
#import "TestSlim.h"

CONTEXT(StatementExecutor)
{
    __block SlimList* empty;
    __block SlimList* args;
    __block StatementExecutor* statementExecutor;

    describe(@"slim class",
             beforeEach(
                ^{
                    empty = SlimList_Create();
                    args = SlimList_Create();
                    statementExecutor = StatementExecutor_Create();
                }),
             it(@"makes an instance of a class with no arguments",
                ^{
                    StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", empty);
                    id test_slim_instance = StatementExecutor_Instance(statementExecutor, "test_slim");

                    bool isTestSlimClass = [test_slim_instance isKindOfClass: [TestSlim class]];
                    expectTruth(isTestSlimClass);
                }),
             it(@"makes an instance of a class with 1 argument",
                ^{
                    SlimList_AddString(args, "starting param");
                    StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", args);
                    TestSlim* test_slim_instance = (TestSlim*)StatementExecutor_Instance(statementExecutor, "test_slim");

                    [expect(test_slim_instance.calledWithStringArg) toBeEqualTo: @"starting param"];
                }),
             it(@"makes an instance of a class with 2 arguments",
                ^{
                    SlimList_AddString(args, "first param");
                    SlimList_AddString(args, "second param");
                    StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", args);
                    TestSlim* test_slim_instance = (TestSlim*)StatementExecutor_Instance(statementExecutor, "test_slim");

                    [expect(test_slim_instance.calledWithFirstStringArg) toBeEqualTo: @"first param"];
                    [expect(test_slim_instance.calledWithSecondStringArg) toBeEqualTo: @"second param"];
                }),
//             it(@"returns an error if the wrong number of arguments is passed for make (1 for 0)",
//                ^{
//                    SlimList* args = SlimList_Create();
//                    SlimList_AddString(args, "starting param");
//                    StatementExecutor* statementExecutor = StatementExecutor_Create();
//
//                    NSString* result = [NSString stringWithFormat: @"%s", StatementExecutor_Make(statementExecutor, "test_slim", "TestSlimTwo", args)];
//
//                    [expect(result) toBeEqualTo: @"__EXCEPTION__:message:<<COULD_NOT_INVOKE_CONSTRUCTOR TestSlimTwo xxx.>>"];
//                }),
             it(@"makes a different instance of the same class",
                ^{
                    StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", empty);
                    StatementExecutor_Make(statementExecutor, "test_slim_2", "TestSlim", empty);
                    TestSlim* test_slim_instance = (TestSlim*)StatementExecutor_Instance(statementExecutor, "test_slim");
                    TestSlim* test_slim_instance_2 = (TestSlim*)StatementExecutor_Instance(statementExecutor, "test_slim_2");
                    StatementExecutor_Call(statementExecutor, "test_slim", "noArgs", args);

                    expectTruth(test_slim_instance.wasNoArgsCalled);
                    expectFalse(test_slim_instance_2.wasNoArgsCalled);
                }),
             it(@"removes an instance when failing to create one of a different name",
                ^{
                    StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", empty);
                    StatementExecutor_Make(statementExecutor, "test_slim", "NoSuchClass", empty);
                    
                    id test_slim_instance = StatementExecutor_Instance(statementExecutor, "test_slim");
                    
                    expectTruth(test_slim_instance == NULL);
                }),
             it(@"returns an error if the instance does not exists",
                ^{
                    NSString* result = [NSString stringWithFormat: @"%s", StatementExecutor_Call(statementExecutor, "missing_instance", "anyMethod", empty)];
                    
                    [expect(result) toBeEqualTo: @"__EXCEPTION__:message:<<The instance missing_instance. does not exist>>"];
                    //        snprintf(errorMessage, 128, "%s", [[NSString stringWithFormat: @"__EXCEPTION__:message:<<The instance %s. does not exist>>", instanceName] UTF8String]);
                }),
             it(@"calls a function with no args",
                ^{
                    StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", empty);
                    StatementExecutor_Call(statementExecutor, "test_slim", "noArgs", args);
                    
                    TestSlim* test_slim_instance = (TestSlim*)StatementExecutor_Instance(statementExecutor, "test_slim");
                    expectTruth(test_slim_instance.wasNoArgsCalled);
                }),
            it(@"returns what the method that takes no arguments returns",
                ^{
                    StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", empty);
                    TestSlim* test_slim_instance = (TestSlim*)StatementExecutor_Instance(statementExecutor, "test_slim");

                    NSString* result = [NSString stringWithFormat: @"%s", StatementExecutor_Call(statementExecutor, "test_slim", "noArgs", args)];
                    
                    [expect(result) toBeEqualTo: [test_slim_instance noArgs]];
                }),
             it(@"returns an error if the function with no arguments does not exist",
                ^{
                    StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", empty);
                    NSString* result = [NSString stringWithFormat: @"%s", StatementExecutor_Call(statementExecutor, "test_slim", "noSuchMethod", args)];
                    
                    [expect(result) toBeEqualTo: @"__EXCEPTION__:message:<<NO_METHOD_IN_CLASS noSuchMethod[0] TestSlim.>>"];
                }),
             it(@"returns an error if calling a method with one agument, but it takes none",
                ^{
                    SlimList_AddString(args, "first");
                    StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", empty);
                    StatementExecutor_Call(statementExecutor, "test_slim", "withStringArg", args);
                    
                    NSString* result = [NSString stringWithFormat: @"%s", StatementExecutor_Call(statementExecutor, "test_slim", "noSuchMethod", args)];
                    [expect(result) toBeEqualTo: @"__EXCEPTION__:message:<<NO_METHOD_IN_CLASS noSuchMethod[1] TestSlim.>>"];
                }),
             it(@"calls a function with a string argument",
                ^{
                    SlimList_AddString(args, "put the toilet seat down");
                    StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", empty);
                    StatementExecutor_Call(statementExecutor, "test_slim", "withStringArg", args);
                    
                    TestSlim* test_slim_instance = (TestSlim*)StatementExecutor_Instance(statementExecutor, "test_slim");
                    [expect(test_slim_instance.calledWithStringArg) toBeEqualTo:@"put the toilet seat down"];
                }),
             it(@"returns an error if the function with one argument does not exist",
                ^{
                    SlimList_AddString(args, "put the toilet seat down");
                    StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", empty);
                    NSString* result = [NSString stringWithFormat: @"%s", StatementExecutor_Call(statementExecutor, "test_slim", "noOtherSuchMethod", args)];
                    
                    [expect(result) toBeEqualTo: @"__EXCEPTION__:message:<<NO_METHOD_IN_CLASS noOtherSuchMethod[1] TestSlim.>>"];
                }),
             it(@"calls a method with null if no arguments are given, but it takes one",
                ^{
                    StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", empty);
                    StatementExecutor_Call(statementExecutor, "test_slim", "withStringArg", args);
                    
                    TestSlim* test_slim_instance = (TestSlim*)StatementExecutor_Instance(statementExecutor, "test_slim");
                    expectTruth(test_slim_instance.calledWithStringArg == NULL);
                }),
             it(@"calls a function with two string arguments",
                ^{
                    SlimList_AddString(args, "put the toilet seat down");
                    SlimList_AddString(args, "every time");
                    StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", empty);
                    StatementExecutor_Call(statementExecutor, "test_slim", "withMultipleArgs", args);
                    
                    TestSlim* test_slim_instance = (TestSlim*)StatementExecutor_Instance(statementExecutor, "test_slim");
                    [expect(test_slim_instance.calledWithFirstStringArg) toBeEqualTo:@"put the toilet seat down"];
                    [expect(test_slim_instance.calledWithSecondStringArg) toBeEqualTo:@"every time"];
                }),
            it(@"returns an error if the class is missing",
               ^{
                   NSString* result = [NSString stringWithFormat: @"%s", StatementExecutor_Make(statementExecutor, "test_slim", "NoSuchClass", empty)];
                   [expect(result) toBeEqualTo: @"__EXCEPTION__:message:<<NO_CLASS NoSuchClass.>>"];
               }),
            it(@"returns the return value of the called method with one argument",
               ^{
                   SlimList_AddString(args, "call with and return this");

                   StatementExecutor* statementExecutor = StatementExecutor_Create();
                   StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", empty);
                   
                   NSString* result = [NSString stringWithFormat: @"%s", StatementExecutor_Call(statementExecutor, "test_slim", "withStringArg", args)];
                   [expect(result) toBeEqualTo: @"return value for one string"];
               }),
             it(@"returns the return value of the called method with multiple arguments",
                ^{
                    SlimList_AddString(args, "first");
                    SlimList_AddString(args, "second");
                    
                    StatementExecutor* statementExecutor = StatementExecutor_Create();
                    StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", empty);
                    
                    NSString* result = [NSString stringWithFormat: @"%s", StatementExecutor_Call(statementExecutor, "test_slim", "withMultipleArgs", args)];
                    [expect(result) toBeEqualTo: @"return value for multiple strings"];
                }),
            it(@"replaces a symbol with it's value",
               ^{
                   SlimList_AddString(args, "hello $v");
                   StatementExecutor_SetSymbol(statementExecutor, "v", "bob");
                   StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", empty);
                   StatementExecutor_Call(statementExecutor, "test_slim", "withStringArg", args);
                   
                   TestSlim* test_slim_instance = (TestSlim*)StatementExecutor_Instance(statementExecutor, "test_slim");
                   [expect(test_slim_instance.calledWithStringArg) toBeEqualTo:@"hello bob"];
               }),
             it(@"replaces a symbol in the middle",
                ^{
                    SlimList_AddString(args, "hello $v person");
                    StatementExecutor_SetSymbol(statementExecutor, "v", "eric");
                    StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", empty);
                    StatementExecutor_Call(statementExecutor, "test_slim", "withStringArg", args);
                    
                    TestSlim* test_slim_instance = (TestSlim*)StatementExecutor_Instance(statementExecutor, "test_slim");
                    [expect(test_slim_instance.calledWithStringArg) toBeEqualTo:@"hello eric person"];
                }),
             it(@"replaces a symbol with other non-alphanumeric",
                ^{
                    SlimList_AddString(args, "$v=why");
                    StatementExecutor_SetSymbol(statementExecutor, "v", "jim");
                    StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", empty);
                    StatementExecutor_Call(statementExecutor, "test_slim", "withStringArg", args);
                    
                    TestSlim* test_slim_instance = (TestSlim*)StatementExecutor_Instance(statementExecutor, "test_slim");
                    [expect(test_slim_instance.calledWithStringArg) toBeEqualTo:@"jim=why"];
                }),
             it(@"replaces multiple different symbols",
                ^{
                    SlimList_AddString(args, "hi $v. Cost:  $12.32 from $e.");
                    StatementExecutor_SetSymbol(statementExecutor, "v", "bob");
                    StatementExecutor_SetSymbol(statementExecutor, "e", "doug");
                    StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", empty);
                    StatementExecutor_Call(statementExecutor, "test_slim", "withStringArg", args);
                    
                    TestSlim* test_slim_instance = (TestSlim*)StatementExecutor_Instance(statementExecutor, "test_slim");
                    [expect(test_slim_instance.calledWithStringArg) toBeEqualTo:@"hi bob. Cost:  $12.32 from doug."];
                }),
             it(@"does not replace a single dollar sign",
                ^{
                    SlimList_AddString(args, "$");
                    StatementExecutor_SetSymbol(statementExecutor, "v", "bob");
                    StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", empty);
                    StatementExecutor_Call(statementExecutor, "test_slim", "withStringArg", args);
                    
                    TestSlim* test_slim_instance = (TestSlim*)StatementExecutor_Instance(statementExecutor, "test_slim");
                    [expect(test_slim_instance.calledWithStringArg) toBeEqualTo:@"$"];
                }),
             it(@"does not replace a dollar sign at the end of the string",
                ^{
                    SlimList_AddString(args, "hi $v$");
                    StatementExecutor* statementExecutor = StatementExecutor_Create();
                    StatementExecutor_SetSymbol(statementExecutor, "v", "bob");
                    StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", empty);
                    StatementExecutor_Call(statementExecutor, "test_slim", "withStringArg", args);
                    
                    TestSlim* test_slim_instance = (TestSlim*)StatementExecutor_Instance(statementExecutor, "test_slim");
                    [expect(test_slim_instance.calledWithStringArg) toBeEqualTo:@"hi bob$"];
                }),
             it(@"replaces a symbol in a sublist",
                ^{
                    
                }),
             it(@"creates an instance with one arg while replacing a symbol",
                ^{
                    SlimList_AddString(args, "hello $person");
                    StatementExecutor_SetSymbol(statementExecutor, "person", "bob");
                    StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", args);
                    TestSlim* test_slim_instance = (TestSlim*)StatementExecutor_Instance(statementExecutor, "test_slim");
                    
                    [expect(test_slim_instance.calledWithStringArg) toBeEqualTo: @"hello bob"];
                }),
             it(@"creates an instance with one arg while replacing two symbols",
                ^{
                    SlimList_AddString(args, "hello $person $animal");
                    StatementExecutor_SetSymbol(statementExecutor, "person", "bob");
                    StatementExecutor_SetSymbol(statementExecutor, "animal", "dog");
                    StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", args);
                    TestSlim* test_slim_instance = (TestSlim*)StatementExecutor_Instance(statementExecutor, "test_slim");
                    
                    [expect(test_slim_instance.calledWithStringArg) toBeEqualTo: @"hello bob dog"];
                }),
             it(@"creates an instance with two args while replacing two symbols",
                ^{
                    SlimList_AddString(args, "hello $person $animal");
                    SlimList_AddString(args, "hello $animal");
                    StatementExecutor_SetSymbol(statementExecutor, "person", "bob");
                    StatementExecutor_SetSymbol(statementExecutor, "animal", "dog");
                    StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", args);
                    TestSlim* test_slim_instance = (TestSlim*)StatementExecutor_Instance(statementExecutor, "test_slim");
                    
                    [expect(test_slim_instance.calledWithFirstStringArg) toBeEqualTo: @"hello bob dog"];
                    [expect(test_slim_instance.calledWithSecondStringArg) toBeEqualTo: @"hello dog"];
                }),
//             it(@"mvkdn",
//                ^{
//                    SlimList* args = SlimList_Create();
//                    SlimList_AddString(args, "Köln");
//                    StatementExecutor* statementExecutor = StatementExecutor_Create();
//                    StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", args);
//                    TestSlim* test_slim_instance = (TestSlim*)StatementExecutor_Instance(statementExecutor, "test_slim");
//                    
//                    [expect(test_slim_instance.calledWithStringArg) toBeEqualTo: @"Köln"];
//                }),
           nil);
}