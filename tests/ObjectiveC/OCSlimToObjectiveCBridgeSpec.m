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

OCDSpec2Context(OCSlimToObjectiveCBridge) {

    Describe(@"Slimlist Query Tables to NSArrays",^{
        
        __block NSArray *result;
        __block SlimList *table;
        
        BeforeEach(^{
            table = SlimList_Create();
        });

        AfterEach(^{
            result = nil;
            //SlimList_Destroy(table);
        });
        
        It(@"converts a query table with only header row to empty array", ^{
            SlimList_AddListWithStrings(table, @[@"Employee Number", @"First Name"]);
            result = SlimListQueryTable_ToNSArray(table);
            [ExpectInt(result.count) toBe:0L];
        });
        
        It(@"converts a query table first row to dictionary of first row", ^{
            SlimList_AddListWithStrings(table, @[@"EmployeeNumber",@"FirstName"]);
            
            // Create Row 1
            SlimList *row1 = SlimList_Create();
            SlimList_AddListWithStrings(row1, @[@"EmployeeNumber",@"1429"]);
            SlimList_AddListWithStrings(row1, @[@"FirstName",@"Bob"]);
            SlimList_AddList(table, row1);
            
            result = SlimListQueryTable_ToNSArray(table);
            [ExpectObj(result[0]) toBeEqualTo:@{@"EmployeeNumber" : @"1429", @"FirstName" : @"Bob" }];
        });
        
        It(@"converts a query table second row to dictionary of second row", ^{
            
            // Create Header Row
            SlimList_AddListWithStrings(table, @[@"EmployeeNumber",@"FirstName"]);

            // Create Row 1
            SlimList *row1 = SlimList_Create();
            SlimList_AddListWithStrings(row1, @[@"EmployeeNumber",@"1429"]);
            SlimList_AddListWithStrings(row1, @[@"FirstName",@"Bob"]);
            SlimList_AddList(table, row1);
            
            // Create Row 2
            SlimList *row2 = SlimList_Create();
            SlimList_AddListWithStrings(row2, @[@"EmployeeNumber",@"8832"]);
            SlimList_AddListWithStrings(row2, @[@"FirstName",@"James"]);;
            SlimList_AddList(table, row2);
            
            result = SlimListQueryTable_ToNSArray(table);
            [ExpectObj(result[1]) toBeEqualTo:@{@"EmployeeNumber":@"8832",@"FirstName":@"James"}];
        });
        
        
    });
    
    
}