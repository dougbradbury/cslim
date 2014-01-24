#import <OCDSpec2/OCDSpec2.h>
#import "OCSObjectiveCtoCBridge.h"
#import "OCSlimToObjectiveCBridge.h"
#import "SlimList.h"

void SlimList_AddListWithStrings(SlimList *self, NSArray *strings){
    SlimList *list = SlimList_Create();
    [strings enumerateObjectsUsingBlock:^(id obj, NSUInteger idx, BOOL *stop) {
        SlimList_AddNSString(list, (NSString*)obj);
    }];
    SlimList_AddList(self, list);
    SlimList_Destroy(list);
}


SlimList * CreateSampleSlimListTable(void){
    SlimList *table = SlimList_Create();
    
    SlimList_AddListWithStrings(table, @[@"employee number"]);
    SlimList_AddListWithStrings(table, @[@"1429"]);
    
    return table;
}


OCDSpec2Context(OCSlimToObjectiveCBridge) {

    Describe(@"Slimlist Query Tables to NSArrays",^{
        
        __block NSArray *result;
        __block SlimList *table;
        
        AfterEach(^{
            result = nil;
            SlimList_Destroy(table);
        });
        
        BeforeEach(^{
            table = SlimList_Create();
        });
        
        It(@"converts a query table with only header row to empty array", ^{
            SlimList_AddListWithStrings(table, @[@"Employee Number", @"First Name"]);
            result = SlimListQueryTable_ToNSArray(table);
            [ExpectInt(result.count) toBe:0L];
        });
        
        It(@"converts a query table first row to dictionary of first row", ^{
            SlimList_AddListWithStrings(table, @[@"EmployeeNumber",@"FirstName"]);
            SlimList_AddListWithStrings(table, @[@"1429",@"Bob"]);
            result = SlimListQueryTable_ToNSArray(table);
            [ExpectObj(result[0]) toBeEqualTo:@{@"EmployeeNumber" : @"1429", @"FirstName" : @"Bob" }];
        });
        
        It(@"converts a query table second row to dictionary of second row", ^{
            SlimList_AddListWithStrings(table, @[@"EmployeeNumber",@"FirstName"]);
            SlimList_AddListWithStrings(table, @[@"1429",@"Bob"]);
            SlimList_AddListWithStrings(table, @[@"8832",@"James"]);
            result = SlimListQueryTable_ToNSArray(table);
            [ExpectObj(result[1]) toBeEqualTo:@{@"EmployeeNumber":@"8832",@"FirstName":@"James"}];
        });
        
        
    });
    
    
}