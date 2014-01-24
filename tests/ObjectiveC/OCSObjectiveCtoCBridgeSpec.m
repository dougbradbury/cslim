#import <OCDSpec2/OCDSpec2.h>
#import "OCSObjectiveCtoCBridge.h"
#import "SlimList.h"
#import "SlimListSerializer.h"

BOOL slimListNotNil(SlimList *list){
    return (list) ? YES : NO;
}

OCDSpec2Context(OCSObjectiveCtoCBridgeSpec) {
    
    Describe(@"converting between c and objective c strings", ^{
        
        It(@"converts an NSString to a char*", ^{
            char* string = NSStringToCString(@"some string");
            [ExpectObj([NSString stringWithUTF8String: string]) toBeEqualTo: @"some string"];
        });
        
        It(@"converts a char* to an NSString*", ^{
            [ExpectObj(CStringToNSString("some string")) toBeEqualTo: @"some string"];
        });
        
        It(@"converts multibyte characters", ^{
            [ExpectObj(CStringToNSString("Köln")) toBeEqualTo: @"Köln"];
        });
        
    });
    
    Describe(@"slim list manipulation", ^{
        
        Describe(@"converting Slim list to an array", ^{
            
            __block SlimList* list;
            
            BeforeEach(^{
                list = SlimList_Create();
            });
            
            AfterEach(^{
                SlimList_Destroy(list);
            });
            
            It(@"converts an empty Slim list into an empty array", ^{
                [ExpectObj(SlimList_ToNSArray(list)) toBeEqualTo: [NSArray array]];
            });
            
            It(@"converts a one element Slim list into an array", ^{
                SlimList_AddString(list, "first");
                [ExpectObj(SlimList_ToNSArray(list)) toBeEqualTo: [NSArray arrayWithObjects: @"first", nil]];
            });
            
            It(@"converts a two element Slim list into an array", ^{
                SlimList_AddString(list, "first");
                SlimList_AddString(list, "second");
                [ExpectObj(SlimList_ToNSArray(list)) toBeEqualTo: [NSArray arrayWithObjects: @"first", @"second", nil]];
            });
            
            It(@"gets the first string in a list", ^{
                SlimList_AddString(list, "first");
                [ExpectObj(SlimList_GetNSStringAt(list, 0)) toBeEqualTo: @"first"];
            });
            
            It(@"gets the second string in a list", ^{
                SlimList_AddString(list, "first");
                SlimList_AddString(list, "second");
                [ExpectObj(SlimList_GetNSStringAt(list, 1)) toBeEqualTo: @"second"];
            });
            
            It(@"gets a string with a multibyte character", ^{
                SlimList_AddString(list, "Köln");
                [ExpectObj(SlimList_GetNSStringAt(list, 0)) toBeEqualTo: @"Köln"];
            });
            
        });
        
        Describe(@"converting NSArray of NSDictionaries to a Slim list",^{
            
            __block SlimList* list;
            
            BeforeEach(^{
                list = nil;
            });
            
            AfterEach(^{
                if(list) SlimList_Destroy(list);
            });
            
            It(@"converts empty dictionary to list with 1 item",^{
                list = NSArray_ToSlimList(@[@{}]);
                [ExpectInt(SlimList_GetLength(list)) toBe:1L];
            });
            
            It(@"converts empty Dictionary to list containing 1 empty list",^{
                list = NSArray_ToSlimList(@[@{}]);
                SlimList *sublist = SlimList_GetListAt(list, 0);
                [ExpectInt(SlimList_GetLength(sublist)) toBe:0L];
            });

            It(@"converts Dictionary with 1 item to list containing 1 item",^{
                list = NSArray_ToSlimList(@[ @{ @"key" : @"value" } ]);
                SlimList *sublist = SlimList_GetListAt(list, 0);
                [ExpectInt(SlimList_GetLength(sublist)) toBe:1L];
            });
            
            It(@"converts Dictionary with 2 items to list containing 2 items",^{
                list = NSArray_ToSlimList(@[ @{ @"key" : @"value", @"key2" : @"value" } ]);
                SlimList *sublist = SlimList_GetListAt(list, 0);
                [ExpectInt(SlimList_GetLength(sublist)) toBe:2L];
            });

            It(@"converts Dictionary with 1 item to list containing 1 list with 2 items",^{
                list = NSArray_ToSlimList(@[ @{ @"key" : @"value" } ]);
                SlimList *pairList = SlimList_GetListAt(SlimList_GetListAt(list, 0), 0);
                [ExpectInt(SlimList_GetLength(pairList)) toBe:2L];
            });
            
            It(@"converts Dictionary with 1 item to list containing 1 list with items for Key Value",^{
                list = NSArray_ToSlimList(@[ @{ @"Key" : @"Value" } ]);
                SlimList *itemList = SlimList_GetListAt(SlimList_GetListAt(list, 0), 0);
                [ExpectObj(SlimList_GetNSStringAt(itemList, 0)) toBeEqualTo:@"Key"];
                [ExpectObj(SlimList_GetNSStringAt(itemList, 1)) toBeEqualTo:@"Value"];
            });
            
            It(@"converts Dictionary with 2 items to list containing 1 lists with 2 items",^{
                list = NSArray_ToSlimList(@[ @{ @"key" : @"value", @"key2" : @"value" } ]);
                SlimList *itemList1 = SlimList_GetListAt(SlimList_GetListAt(list, 0), 0);
                SlimList *itemList2 = SlimList_GetListAt(SlimList_GetListAt(list, 0), 1);
                [ExpectInt(SlimList_GetLength(itemList1)) toBe:2L];
                [ExpectInt(SlimList_GetLength(itemList2)) toBe:2L];
            });
            
            It(@"converts 2 Dictionaries with 1 item to list containing 2 items", ^{
                list = NSArray_ToSlimList(@[ @{ @"key" : @"value" }, @{ @"key" : @"value" } ]);
                [ExpectInt(SlimList_GetLength(list)) toBe:2L];
            });
            
            It(@"converts 2 Dictionaries with 2 items to list containing 2 lists with 2 lists with items for Key Values",^{
                list = NSArray_ToSlimList(@[ @{@"employee number":@(1429), @"first name":@"Bob"},
                                             @{@"employee number":@(8832), @"first name":@"James"}
                                             ]);
                SlimList *dictionaryA = SlimList_GetListAt(SlimList_GetListAt(list, 0), 0);
                SlimList *dictionaryB = SlimList_GetListAt(SlimList_GetListAt(list, 0), 1);
                [ExpectObj(SlimList_GetNSStringAt(dictionaryA, 0)) toBeEqualTo:@"employee number"];
                [ExpectObj(SlimList_GetNSStringAt(dictionaryA, 1)) toBeEqualTo:@"1429"];
                [ExpectObj(SlimList_GetNSStringAt(dictionaryB, 0)) toBeEqualTo:@"first name"];
                [ExpectObj(SlimList_GetNSStringAt(dictionaryB, 1)) toBeEqualTo:@"Bob"];
                SlimList *dictionaryAA = SlimList_GetListAt(SlimList_GetListAt(list, 1), 0);
                SlimList *dictionaryBB = SlimList_GetListAt(SlimList_GetListAt(list, 1), 1);
                [ExpectObj(SlimList_GetNSStringAt(dictionaryAA, 0)) toBeEqualTo:@"employee number"];
                [ExpectObj(SlimList_GetNSStringAt(dictionaryAA, 1)) toBeEqualTo:@"8832"];
                [ExpectObj(SlimList_GetNSStringAt(dictionaryBB, 0)) toBeEqualTo:@"first name"];
                [ExpectObj(SlimList_GetNSStringAt(dictionaryBB, 1)) toBeEqualTo:@"James"];
            });
        });
        
        Describe(@"converting NSArray to a Slim list",^{
            
            __block SlimList* list;
        
            BeforeEach(^{
                list = nil;
            });
            
            AfterEach(^{
                if(list) SlimList_Destroy(list);
            });
            
            It(@"converts a nil into an empty list",^{
                list = NSArray_ToSlimList(nil);
                int result = SlimList_Equals(list, SlimList_Create());
                [ExpectInt(result) toBeTrue];
            });
            
            It(@"converts Array with String into list of 1",^{
                list = NSArray_ToSlimList(@[@"1"]);
                int result = SlimList_GetLength(list);
                [ExpectInt(result) toBe:1L];
            });
            
            It(@"converts Array with Strings into list of 2",^{
                list = NSArray_ToSlimList(@[@"1",@"2"]);
                int result = SlimList_GetLength(list);
                [ExpectInt(result) toBe:2L];
            });
            
            It(@"converts Array with Strings into list of 1",^{
                list = NSArray_ToSlimList(@[@"1"]);
                NSString* result = SlimList_GetNSStringAt(list, 0);
                [ExpectObj(result) toBeEqualTo:@"1"];
            });
            
            It(@"converts Array with Number into list of 1 string 1",^{
                list = NSArray_ToSlimList(@[@(1)]);
                NSString* result = SlimList_GetNSStringAt(list, 0);
                [ExpectObj(result) toBeEqualTo:@"1"];
            });
            
            It(@"converts Array with Boolean into list of 1 string of true",^{
                list = NSArray_ToSlimList(@[@YES]);
                NSString* result = SlimList_GetNSStringAt(list, 0);
                [ExpectObj(result) toBeEqualTo:@"true"];
            });
            
        });
        
        
        Describe(@"converting NSDictionary to a slim list",^{
           
            __block SlimList* list;
            
            BeforeEach(^{
                list = nil;
            });
            
            AfterEach(^{
                if(list) SlimList_Destroy(list);
            });
            
            It(@"returns empty list for an empty dictionary ",^{
                list = NSDictionary_ToSlimList(@{});
                [ExpectInt(SlimList_GetLength(list)) toBe:0L];
            });

            It(@"converts a dictionary with 1 key value pair into a slim list with count of 1",^{
                list = NSDictionary_ToSlimList( @{@"key":@"value"} );
                SlimList * slimlist = SlimList_GetListAt(list, 0);
                NSString *key = SlimList_GetNSStringAt(slimlist, 0);
                NSString *value = SlimList_GetNSStringAt(slimlist, 1);
                [ExpectObj(key) toBeEqualTo:@"key"];
                [ExpectObj(value) toBeEqualTo:@"value"];
            });
            
            It(@"converts a dictionary with 2 key value pair into a slim list of 2 list",^{
                list = NSDictionary_ToSlimList( @{@"key":@"value", @"key2":@"value2"} );
                SlimList * itemA = SlimList_GetListAt(list, 0);
                SlimList * itemB = SlimList_GetListAt(list, 1);
                ExpectTrue(slimListNotNil(itemA));
                ExpectTrue(slimListNotNil(itemB));
            });
            
            It(@"converts a dictionary with number value into a slim list with a string value",^{
                list = NSDictionary_ToSlimList( @{ @"number" : @(123), @"string" : @"abc" } );
                SlimList * itemA = SlimList_GetListAt(list, 0);
                SlimList * itemB = SlimList_GetListAt(list, 1);
                [ExpectObj(SlimList_GetNSStringAt(itemA, 1)) toBeEqualTo:@"123"];
                [ExpectObj(SlimList_GetNSStringAt(itemB, 1)) toBeEqualTo:@"abc"];
            });
            
        });

    });
}
