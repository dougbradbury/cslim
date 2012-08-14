#import <Foundation/Foundation.h>
#import "OCSSymbolDictionary.h"

@interface OCSStatementExecutor : NSObject

@property (nonatomic, strong) OCSSymbolDictionary* symbolDictionary;
@property (nonatomic, strong) NSMutableDictionary* instances;

+(id) sharedExecutor;
-(void) reset;

-(id) instanceWithName:(NSString*) instanceName;
-(void) removeInstanceWithName:(NSString*) instanceName;

-(NSString*) makeInstanceWithName:(NSString*) instanceName
                        className:(NSString*) className
                          andArgs:(NSArray*) args;

-(NSString*) callMethod:(NSString*) methodName
     onInstanceWithName:(NSString*) instanceName
               withArgs:(NSArray*) args;

-(void) setSymbol:(NSString*) symbol toValue:(NSString*) value;

@end
