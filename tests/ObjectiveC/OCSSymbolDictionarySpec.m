#import <SpecKit/SpecKit.h>
#import "OCSSymbolDictionary.h"

SpecKitContext(OCSSymbolDictionarySpec) {

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
        
        __block NSMutableArray* stringList;
        __block NSArray* updatedList;
        
        BeforeEach(^{
            symbolDictionary = [OCSSymbolDictionary new];
            stringList = [NSMutableArray array];
        });
        
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
    
}
