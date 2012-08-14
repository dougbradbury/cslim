#import <Foundation/Foundation.h>

@interface OCSSymbolDictionary : NSObject

@property(nonatomic, strong) NSMutableDictionary* symbols;

-(void) setSymbol:(NSString*) symbol toValue:(NSString*) value;

-(NSString*) replaceSymbolsInString:(NSString*) givenString;
-(NSArray*) replaceSymbolsInArray:(NSArray*) args;

@end
