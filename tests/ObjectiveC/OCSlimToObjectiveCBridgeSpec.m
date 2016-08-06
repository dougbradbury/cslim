#import <OCDSpec2/OCDSpec2.h>
#import "OCSObjectiveCtoCBridge.h"
#import "OCSlimToObjectiveCBridge.h"
#import "SlimList.h"
#import "SlimListDeserializer.h"

void SlimList_AddListWithStrings(SlimList *self, NSArray *strings){
    SlimList *list = SlimList_Create();
    [strings enumerateObjectsUsingBlock:^(id obj, NSUInteger idx, BOOL *stop) {
        SlimList_AddNSString(list, (NSString*)obj);
    }];
    SlimList_AddList(self, list);
    SlimList_Destroy(list);
}

OCDSpec2Context(OCSlimToObjectiveCBridge) {

    Describe(@"Slimlist Query Tables to NSArrays",^{
        
        __block NSArray *result;
        __block SlimList *table;
        __block SlimList *tableContent;
        
        BeforeEach(^{
            table = SlimList_Create();
            tableContent = SlimList_Create();
        });

        AfterEach(^{
            result = nil;
        });
        
        It(@"converts a query table with only header row to empty array", ^{
            SlimList_AddListWithStrings(tableContent, @[@"Employee Number", @"First Name"]);
            SlimList_AddList(table, tableContent);
            
            result = SlimListQueryTable_ToNSArray(table);
            [ExpectInt(result.count) toBe:0L];
        });

        It(@"converts a query table first row to dictionary of first row", ^{
            SlimList_AddListWithStrings(tableContent, @[@"EmployeeNumber",@"FirstName"]);
            SlimList_AddListWithStrings(tableContent, @[@"1429",@"Bob"]);
            SlimList_AddList(table, tableContent);
            
            result = SlimListQueryTable_ToNSArray(table);
            [ExpectObj(result[0]) toBeEqualTo:@{@"EmployeeNumber" : @"1429", @"FirstName" : @"Bob" }];
        });
        It(@"converts a query table second row to dictionary of second row", ^{
            SlimList_AddListWithStrings(tableContent, @[@"EmployeeNumber",@"FirstName"]);
            SlimList_AddListWithStrings(tableContent, @[@"1429",@"Bob"]);
            SlimList_AddListWithStrings(tableContent, @[@"8832",@"James"]);
            SlimList_AddList(table, tableContent);
            
            result = SlimListQueryTable_ToNSArray(table);
            [ExpectObj(result[1]) toBeEqualTo:@{@"EmployeeNumber":@"8832",@"FirstName":@"James"}];
        });

        It(@"converts a serialized slimlist table to an array",^{
            SlimList *table = SlimList_Deserialize("[000001:000195:[000003:000067:[000003:000015:Employee Number:000010:First Name:000009:Last Name:]:000046:[000003:000004:1429:000003:Bob:000006:Martin:]:000049:[000003:000004:9924:000004:Bill:000008:Mitchell:]:]:]");
            result = SlimListQueryTable_ToNSArray(table);
            [ExpectObj(result[0]) toBeEqualTo:@{@"Employee Number":@"1429",@"First Name":@"Bob",@"Last Name":@"Martin"}];
            
        });
        
    });
    
    
}