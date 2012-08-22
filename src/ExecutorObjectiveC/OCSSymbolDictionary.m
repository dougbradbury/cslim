#import "OCSSymbolDictionary.h"

@interface OCSSymbolDictionary ()

@property(nonatomic, strong) NSMutableDictionary* symbols;

@end

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
        newString = [newString stringByReplacingOccurrencesOfString: [NSString stringWithFormat: @"$%@", symbol]
                                                         withString: [self.symbols objectForKey: symbol]];
    }
    return newString;
}

-(NSArray*) replaceSymbolsInArray:(NSArray*) args {
    NSMutableArray* updatedArgs = [NSMutableArray array];
    for (NSString* arg in args) {
        [updatedArgs addObject: [self replaceSymbolsInString: arg]];
    }
    return updatedArgs;
}

@end
