#import <OCDSpec2/OCDSpec2.h>
#import "OCSSymbolDictionary.h"

OCDSpec2Context(OCSSymbolDictionarySpec) {
    
    __block OCSSymbolDictionary* symbolDictionary;
    
    Describe(@"replaceSymbolsInString", ^{
        
        BeforeEach(^{
            symbolDictionary = [OCSSymbolDictionary new];
        });
        
        It(@"replaces a symbol with it's value", ^{
            [symbolDictionary setSymbol: @"v" toValue: @"bob"];
            
            NSString* replacedString = [symbolDictionary replaceSymbolsInString: @"hello $v"];
            [ExpectObj(replacedString) toBeEqualTo: @"hello bob"];
        });
        
        It(@"replaces a symbol in the middle", ^{
            [symbolDictionary setSymbol: @"v" toValue: @"eric"];
            
            NSString* replacedString = [symbolDictionary replaceSymbolsInString: @"hello $v person"];
            [ExpectObj(replacedString) toBeEqualTo: @"hello eric person"];
        });
        
        It(@"replaces a symbol with other non-alphanumeric", ^{
            [symbolDictionary setSymbol: @"v" toValue: @"jim"];
            
            NSString* replacedString = [symbolDictionary replaceSymbolsInString: @"$v=why"];
            [ExpectObj(replacedString) toBeEqualTo: @"jim=why"];
        });
        
        It(@"replaces multicharacter symbols", ^{
            [symbolDictionary setSymbol: @"foobar" toValue: @"eric"];
            
            NSString* replacedString = [symbolDictionary replaceSymbolsInString: @"hi $foobar person"];
            [ExpectObj(replacedString) toBeEqualTo: @"hi eric person"];
        });
        
        It(@"replaces multiple different symbols", ^{
            [symbolDictionary setSymbol: @"v" toValue: @"bob"];
            [symbolDictionary setSymbol: @"e" toValue: @"doug"];
            
            NSString* replacedString = [symbolDictionary replaceSymbolsInString: @"hi $v. Cost:  $12.32 from $e."];
            [ExpectObj(replacedString) toBeEqualTo: @"hi bob. Cost:  $12.32 from doug."];
        });
        
        It(@"does not replace a single dollar sign", ^{
            [symbolDictionary setSymbol: @"v" toValue: @"bob"];
            
            NSString* replacedString = [symbolDictionary replaceSymbolsInString: @"$"];
            [ExpectObj(replacedString) toBeEqualTo:@"$"];
        });
        
        It(@"does not replace a dollar sign at the end of the string", ^{
            [symbolDictionary setSymbol: @"someone" toValue: @"bob"];
            
            NSString* replacedString = [symbolDictionary replaceSymbolsInString: @"hi $someone$"];
            [ExpectObj(replacedString) toBeEqualTo:@"hi bob$"];
        });
        
        It(@"keeps strange characters in a symbol", ^{
            [symbolDictionary setSymbol: @"v" toValue: @"Köln"];
            
            NSString* replacedString = [symbolDictionary replaceSymbolsInString: @"$v"];
            [ExpectObj(replacedString) toBeEqualTo: @"Köln"];
        });
        
        It(@"keeps strange characters in the given string", ^{
            [symbolDictionary setSymbol: @"v" toValue: @"hi"];
            
            NSString* replacedString = [symbolDictionary replaceSymbolsInString: @"$v Köln"];
            [ExpectObj(replacedString) toBeEqualTo: @"hi Köln"];
        });
        
    });
    
    Describe(@"replaceSymbolsInArray", ^{
        
        __block NSArray* updatedList;
        __block NSMutableArray* stringList;
        
        BeforeEach(^{
            symbolDictionary = [OCSSymbolDictionary new];
            stringList = [NSMutableArray array];
        });
        
        Describe(@"list of strings", ^{
            
            It(@"replaces nothing for an empty array", ^{
                updatedList = [symbolDictionary replaceSymbolsInArray: stringList];
                
                [ExpectObj(updatedList) toBeEqualTo: [NSArray array]];
            });
            
            It(@"replaces one symbol in one string", ^{
                [stringList addObject: @"hi $foo person"];
                [symbolDictionary setSymbol: @"foo" toValue: @"eric"];
                updatedList = [symbolDictionary replaceSymbolsInArray: stringList];
                
                [ExpectObj(updatedList) toBeEqualTo: [NSArray arrayWithObjects: @"hi eric person", nil]];
            });
            
            It(@"replaces two symbols in one string", ^{
                [stringList addObject: @"hi $foo $bar person"];
                [symbolDictionary setSymbol: @"foo" toValue: @"eric"];
                [symbolDictionary setSymbol: @"bar" toValue: @"m"];
                updatedList = [symbolDictionary replaceSymbolsInArray: stringList];
                
                [ExpectObj(updatedList) toBeEqualTo: [NSArray arrayWithObjects: @"hi eric m person", nil]];
            });
            
            It(@"replaces one symbol in two strings", ^{
                [stringList addObject: @"hi $foo person"];
                [stringList addObject: @"hi $bar person"];
                [symbolDictionary setSymbol: @"foo" toValue: @"eric"];
                [symbolDictionary setSymbol: @"bar" toValue: @"bob"];
                updatedList = [symbolDictionary replaceSymbolsInArray: stringList];
                
                NSArray* expectedList = [NSArray arrayWithObjects: @"hi eric person", @"hi bob person", nil];
                [ExpectObj(updatedList) toBeEqualTo: expectedList];
            });
            
            It(@"replaces two symbols in two strings", ^{
                [stringList addObject: @"hi $foo $bar person"];
                [stringList addObject: @"hi $baz $bar person"];
                [symbolDictionary setSymbol: @"foo" toValue: @"eric"];
                [symbolDictionary setSymbol: @"bar" toValue: @"bob"];
                [symbolDictionary setSymbol: @"baz" toValue: @"dough"];
                updatedList = [symbolDictionary replaceSymbolsInArray: stringList];
                
                NSArray* expectedList = [NSArray arrayWithObjects: @"hi eric bob person", @"hi dough bob person", nil];
                [ExpectObj(updatedList) toBeEqualTo: expectedList];
            });
            
        });
        
        
        Describe(@"list of strings within a list",^{
            
            __block NSMutableArray* arrayList;
            
            BeforeEach(^{
                symbolDictionary = [OCSSymbolDictionary new];
                stringList = [NSMutableArray array];
                arrayList = [NSMutableArray array];
                [arrayList addObject:stringList];
            });
            
            It(@"replaces an empty string list for an empty list", ^{
                updatedList = [symbolDictionary replaceSymbolsInArray: arrayList];
                
                [ExpectObj(updatedList) toBeEqualTo: @[ @[] ]];
            });
            
            It(@"replaces a string list without symbols for the same list", ^{
                [stringList addObject: @"hi eric person"];
                updatedList = [symbolDictionary replaceSymbolsInArray: arrayList];
                
                [ExpectObj(updatedList) toBeEqualTo: @[ @[ @"hi eric person"] ]];
            });
            
            It(@"replaces one symbol in one string within a list ",^{
                [stringList addObject: @"hi $foo person"];
                [symbolDictionary setSymbol: @"foo" toValue: @"bob"];
                updatedList = [symbolDictionary replaceSymbolsInArray: arrayList];
                
                [ExpectObj(updatedList) toBeEqualTo: @[ @[ @"hi bob person"] ]];
            });
            
            It(@"replaces two symbols in one string", ^{
                [stringList addObject: @"hi $foo $bar person"];
                [symbolDictionary setSymbol: @"foo" toValue: @"eric"];
                [symbolDictionary setSymbol: @"bar" toValue: @"m"];
                updatedList = [symbolDictionary replaceSymbolsInArray: arrayList];
                
                [ExpectObj(updatedList) toBeEqualTo: @[ @[@"hi eric m person"] ] ];
            });
            
            It(@"replaces one symbol in two strings", ^{
                [stringList addObject: @"hi $foo person"];
                [stringList addObject: @"hi $bar person"];
                [symbolDictionary setSymbol: @"foo" toValue: @"eric"];
                [symbolDictionary setSymbol: @"bar" toValue: @"bob"];
                updatedList = [symbolDictionary replaceSymbolsInArray: arrayList];
                
                NSArray* expectedList = @[ @[@"hi eric person", @"hi bob person"] ];
                [ExpectObj(updatedList) toBeEqualTo: expectedList];
            });
            
            It(@"replaces two symbols in two strings", ^{
                [stringList addObject: @"hi $foo $bar person"];
                [stringList addObject: @"hi $baz $bar person"];
                [symbolDictionary setSymbol: @"foo" toValue: @"eric"];
                [symbolDictionary setSymbol: @"bar" toValue: @"bob"];
                [symbolDictionary setSymbol: @"baz" toValue: @"dough"];
                updatedList = [symbolDictionary replaceSymbolsInArray: arrayList];
                
                NSArray* expectedList = @[ @[ @"hi eric bob person", @"hi dough bob person" ] ];
                [ExpectObj(updatedList) toBeEqualTo: expectedList];
            });
            
        });
        
        Describe(@"dictionary values within a list",^{
            
            __block NSMutableArray* arrayList;
            __block NSMutableDictionary* dictionary;
            
            BeforeEach(^{
                symbolDictionary = [OCSSymbolDictionary new];
                dictionary = [NSMutableDictionary dictionary];
                arrayList = [NSMutableArray array];
                [arrayList addObject:dictionary];
            });
            
            It(@"replaces an empty dictionary for an empty list", ^{
                updatedList = [symbolDictionary replaceSymbolsInArray: arrayList];
                
                [ExpectObj(updatedList) toBeEqualTo: @[ @{} ]];
            });
            
            It(@"replaces a dictionary value without symbols in a list", ^{
                [dictionary setValue:@"hi eric person" forKey:@"key"];
                updatedList = [symbolDictionary replaceSymbolsInArray: arrayList];
                
                [ExpectObj(updatedList) toBeEqualTo: @[ @{ @"key" : @"hi eric person" } ]];
            });

            It(@"replaces one symbol in one string within a list ",^{
                [dictionary setValue:@"hi $foo person" forKey:@"key"];
                [symbolDictionary setSymbol: @"foo" toValue: @"bob"];
                updatedList = [symbolDictionary replaceSymbolsInArray: arrayList];
                
                [ExpectObj(updatedList) toBeEqualTo: @[ @{ @"key" : @"hi bob person" } ]];
            });

            It(@"replaces two symbols in one string", ^{
                [dictionary setValue: @"hi $foo $bar person" forKey:@"key"];
                [symbolDictionary setSymbol: @"foo" toValue: @"eric"];
                [symbolDictionary setSymbol: @"bar" toValue: @"m"];
                updatedList = [symbolDictionary replaceSymbolsInArray: arrayList];
                
                [ExpectObj(updatedList) toBeEqualTo: @[ @{ @"key" : @"hi eric m person" } ] ];
            });

            It(@"replaces one symbol in two strings", ^{
                [dictionary setValue: @"hi $foo person" forKey:@"keyA"];
                [dictionary setValue: @"hi $bar person" forKey:@"keyB"];
                [symbolDictionary setSymbol: @"foo" toValue: @"eric"];
                [symbolDictionary setSymbol: @"bar" toValue: @"bob"];
                updatedList = [symbolDictionary replaceSymbolsInArray: arrayList];
                
                NSArray* expectedList = @[ @{@"keyA" : @"hi eric person",  @"keyB" : @"hi bob person"} ];
                [ExpectObj(updatedList) toBeEqualTo: expectedList];
            });

            
        });

        
        
    });
    
}
