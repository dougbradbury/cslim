#import "OCSStatementExecutor.h"
#import "OCSReturnValue.h"
#import "OCSException.h"
#import "OCSSymbolDictionary.h"
#import "OCSInstance.h"

static OCSStatementExecutor* sharedExecutor = NULL;

@implementation OCSStatementExecutor

+(id) sharedExecutor {
    if (sharedExecutor == NULL) {
        sharedExecutor = [self new];
    }
    return sharedExecutor;
}

-(id) init {
    if ((self = [super init])) {
        [self reset];
    }
    return self;
}

-(void) reset {
    self.instances = [NSMutableDictionary dictionary];
    self.symbolDictionary = [OCSSymbolDictionary new];
}

-(id) instanceWithName:(NSString*) instanceName {
    return [self.instances valueForKey: instanceName];
}

-(void) removeInstanceWithName:(NSString*) instanceName {
    [self.instances removeObjectForKey: instanceName];
    
}

-(NSString*) makeInstanceWithName:(NSString*) instanceName
                        className:(NSString*) className
                          andArgs:(NSArray*) args {
    OCSInstance* ocsInstance = [OCSInstance instanceWithName: instanceName
                                                   ClassName: [self.symbolDictionary replaceSymbolsInString: className]
                                                     andArgs: [self.symbolDictionary replaceSymbolsInArray: args]];
    id createdInstance = [ocsInstance create];
    [self.instances setValue: createdInstance forKey: instanceName];
    return [ocsInstance result];
}

-(NSString*) callMethod:(NSString*) methodName
     onInstanceWithName:(NSString*) instanceName
               withArgs:(NSArray*) args {
    id instance = [self instanceWithName: instanceName];
    int length = (int)[args count];
    SEL selector = [self selectorFromNSString: methodName andLength: length];
    if(instance == NULL) {
        return [[OCSException exceptionWithMessage: @"The instance %@. does not exist", instanceName] stringValue];
    }
    if(![instance respondsToSelector: selector]) {
        return [[OCSException exceptionWithMessage: @"NO_METHOD_IN_CLASS %@[%i] TestSlim.",
                 methodName,
                 length] stringValue];
    }
    NSArray* updatedArgs = [self.symbolDictionary replaceSymbolsInArray: args];
    NSMethodSignature* signature = [instance methodSignatureForSelector: selector];
    id result;
    @try {
        if(length == 0) {
            result = [instance performSelector: selector];
        } else if (length == 1) {
            result = [instance performSelector: selector withObject: [updatedArgs objectAtIndex: 0]];
        } else {
            result = [instance performSelector: selector withObject: updatedArgs];
        }
        return [OCSReturnValue forObjectOrPrimitive: result andMethodSignature: signature];
    } @catch (NSException* e) {
        return [[OCSException exceptionWithNSException: e] stringValue];
    }
    
}

-(SEL) selectorFromNSString:(NSString*) methodName andLength:(int) numberOrArguments {
    if (numberOrArguments == 0) {
        return NSSelectorFromString(methodName);
    } else {
        return NSSelectorFromString([NSString stringWithFormat:@"%@:", methodName]);
    }
}

-(void) setSymbol:(NSString*) symbol toValue:(NSString*) value {
    [self.symbolDictionary setSymbol: symbol toValue: value];
}

@end
