#import <OCDSpec2/OCDSpec2.h>
#import "StatementExecutor.h"
#import "TestSlim.h"
#import "OCSlimToObjectiveCBridge.h"

OCDSpec2Context(StatementExecutor)
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
            TestSlim* test_slim_instance = (__bridge TestSlim*)StatementExecutor_Instance(statementExecutor, "test_slim");

            [ExpectObj(test_slim_instance.calledWithStringArg) toBeEqualTo: @"starting param"];
        });

        It(@"calls a function with a string argument", ^{
            SlimList_AddString(args, "first arg");
            StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", empty);
            StatementExecutor_Call(statementExecutor, "test_slim", "withStringArg", args);

            TestSlim* test_slim_instance = (__bridge TestSlim*)StatementExecutor_Instance(statementExecutor, "test_slim");
            [ExpectObj(test_slim_instance.calledWithStringArg) toBeEqualTo: @"first arg"];
        });

        It(@"returns the return value of the called method with one argument", ^{
            SlimList_AddString(args, "call with and return this");

            StatementExecutor* statementExecutor = StatementExecutor_Create();
            StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", empty);

            NSString* result = [NSString stringWithUTF8String: StatementExecutor_Call(statementExecutor, "test_slim", "withStringArg", args)];
            [ExpectObj(result) toBeEqualTo: @"return value for one string"];
        });

        It(@"creates an instance with one arg while replacing two symbols", ^{
            SlimList_AddString(args, "hello $person $animal");
            StatementExecutor_SetSymbol(statementExecutor, "person", "bob");
            StatementExecutor_SetSymbol(statementExecutor, "animal", "dog");
            StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", args);
            TestSlim* test_slim_instance = (__bridge TestSlim*)StatementExecutor_Instance(statementExecutor, "test_slim");

            [ExpectObj(test_slim_instance.calledWithStringArg) toBeEqualTo: @"hello bob dog"];
        });

        It(@"creates an instance with two args while replacing two symbols", ^{
            SlimList_AddString(args, "hello $person $animal");
            SlimList_AddString(args, "hello $animal");
            StatementExecutor_SetSymbol(statementExecutor, "person", "bob");
            StatementExecutor_SetSymbol(statementExecutor, "animal", "dog");
            StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", args);
            TestSlim* test_slim_instance = (__bridge TestSlim*)StatementExecutor_Instance(statementExecutor, "test_slim");

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
            TestSlim* test_slim_instance = (__bridge TestSlim*)StatementExecutor_Instance(statementExecutor, "test_slim");
            [ExpectObj(result) toBeEqualTo: [[test_slim_instance returnsNSNumber] stringValue]];
        });

        It(@"handles multibyte characters", ^{
            SlimList* args = SlimList_Create();
            SlimList_AddString(args, "Köln");
            StatementExecutor* statementExecutor = StatementExecutor_Create();
            StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", args);
            TestSlim* test_slim_instance = (__bridge TestSlim*)StatementExecutor_Instance(statementExecutor, "test_slim");

            [ExpectObj(test_slim_instance.calledWithStringArg) toBeEqualTo: @"Köln"];
        });
        
        It(@"when converts a slimlist table with no rows calls table with empty array",^{
            
            SlimList* table = SlimList_Create();
            SlimList* tableContents = SlimList_Create();
            
            SlimList* header = SlimList_Create();
            SlimList_AddString(header, "FirstName");
            SlimList_AddList(tableContents, header);
            SlimList_AddList(table, tableContents);
        
            StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", table);
            StatementExecutor_Call(statementExecutor, "test_slim", "table", table);
            TestSlim* test_slim_instance = (__bridge TestSlim*)StatementExecutor_Instance(statementExecutor, "test_slim");

            [ExpectObj(test_slim_instance.calledWithTable) toBeEqualTo:@[]];
            
        });
        
        It(@"converts a table method call to a NSArray representation of table",^{
            
            SlimList *table = SlimList_Create();
            SlimList* tableContents = SlimList_Create();
            
            SlimList *header = SlimList_Create();
            SlimList_AddString(header, "FirstName");
            SlimList_AddList(tableContents, header);
            
            SlimList *row = SlimList_Create();
            SlimList_AddString(row, "Bob");
            SlimList_AddList(tableContents, row);
            
            SlimList_AddList(table, tableContents);
            
            StatementExecutor_Make(statementExecutor, "test_slim", "TestSlim", table);
            StatementExecutor_Call(statementExecutor, "test_slim", "table", table);
            TestSlim* test_slim_instance = (__bridge TestSlim*)StatementExecutor_Instance(statementExecutor, "test_slim");
        
            [ExpectObj(test_slim_instance.calledWithTable) toBeEqualTo:@[ @{@"FirstName":@"Bob"} ]];
            
        });
        
    });
}