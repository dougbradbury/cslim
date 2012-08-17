#import <SpecKit/SpecKit.h>
#import "OCSObjectiveCtoCBridge.h"
#import "SlimList.h"

SpecKitContext(OCSObjectiveCtoCBridgeSpec) {
    
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
}
