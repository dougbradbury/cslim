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
        
        It(@"makes an instance of a class with 1 argument", ^{
            SlimList_AddString(args, "starting param");
            StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", args);
            TestSlim* test_slim_instance = (TestSlim*)StatementExecutor_Instance(statementExecutor, "test_slim");
            
            [ExpectObj(test_slim_instance.calledWithStringArg) toBeEqualTo: @"starting param"];
        });
        
        
        It(@"calls a function with a string argument", ^{
            SlimList_AddString(args, "first arg");
            StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", empty);
            StatementExecutor_Call(statementExecutor, "test_slim", "withStringArg", args);
            
            TestSlim* test_slim_instance = (TestSlim*)StatementExecutor_Instance(statementExecutor, "test_slim");
            [ExpectObj(test_slim_instance.calledWithStringArg) toBeEqualTo: @"first arg"];
        });
        
        It(@"returns the return value of the called method with one argument", ^{
            SlimList_AddString(args, "call with and return this");
            
            StatementExecutor* statementExecutor = StatementExecutor_Create();
            StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", empty);
            
            NSString* result = [NSString stringWithFormat: @"%s", StatementExecutor_Call(statementExecutor, "test_slim", "withStringArg", args)];
            [ExpectObj(result) toBeEqualTo: @"return value for one string"];
        });

        /* end of what was brought over*/
                        
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

        It(@"handles strange characters", ^{
//            SlimList* args = SlimList_Create();
//            SlimList_AddString(args, "Köln");
//            StatementExecutor* statementExecutor = StatementExecutor_Create();
//            StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", args);
//            TestSlim* test_slim_instance = (TestSlim*)StatementExecutor_Instance(statementExecutor, "test_slim");
//            
//            [ExpectObj(test_slim_instance.calledWithStringArg) toBeEqualTo: @"Köln"];
        });
    });
}