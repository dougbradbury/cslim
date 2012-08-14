#import "OCSSymbolDictionary.h"

@implementation OCSSymbolDictionary

-(id) init {
    if ((self = [super init])) {
        self.symbols = [NSMutableDictionary dictionary];
    }
    return self;
}

-(void) setSymbol:(NSString*) symbol toValue:(NSString*) value {
    [self.symbols setObject: value forKey: symbol];
}

-(NSString*) replaceSymbolsInString:(NSString*) givenString {
    NSString* newString = givenString;
    for(NSString* symbol in [[self.symbols keyEnumerator] allObjects]) {
        newString = [givenString stringByReplacingOccurrencesOfString: [NSString stringWithFormat: @"$%@", symbol]
                                                           withString: [self.symbols objectForKey: symbol]];
    }
    return newString;
}

-(NSArray*) replaceSymbolsInArray:(NSArray*) args {
    NSMutableArray* updatedArgs = [NSMutableArray arrayWithArray: args];
    for(int i=0; i<[updatedArgs count]; i++) {
        NSString* newArgument = [updatedArgs objectAtIndex: i];
        for(NSString* symbol in [[self.symbols keyEnumerator] allObjects]) {
            newArgument = [newArgument stringByReplacingOccurrencesOfString: [NSString stringWithFormat: @"$%@", symbol]
                                                                 withString: [self.symbols objectForKey: symbol]];
            [updatedArgs replaceObjectAtIndex: i withObject: newArgument];
        }
    }
    return updatedArgs;
}

@end
