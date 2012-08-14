#import "OCSStatementExecutor.h"
#import "OCSReturnValue.h"
#import "OCSException.h"
#import "OCSSymbolDictionary.h"

static OCSStatementExecutor* sharedExecutor = NULL;

@implementation OCSStatementExecutor

+(id) sharedExecutor {
    if (sharedExecutor == NULL) {
        sharedExecutor = [self new];
    }
    return sharedExecutor;
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
    NSString* newClassName = [self.symbolDictionary replaceSymbolsInString: className];
    Class class = NSClassFromString(newClassName);
    if(class == nil) {
        [[OCSStatementExecutor sharedExecutor] removeInstanceWithName: instanceName];
        return [[OCSException exceptionWithMessage: @"NO_CLASS %@.", newClassName] stringValue];
    } else {
        args = [self.symbolDictionary replaceSymbolsInArray: args];
        @try {
            id instance;
            switch ([args count]) {
                case 0:
                    instance = [[class alloc] init];
                    break;
                case 1:
                    instance = [[class alloc] initWithString: [args objectAtIndex: 0]];
                    break;
                default:
                    instance = [[class alloc] initWithArray: args];
                    break;
            }
            [[[OCSStatementExecutor sharedExecutor] instances] setValue: instance forKey: instanceName];
        }
        @catch (NSException* exception) {
            return [[OCSException exceptionWithMessage: @"COULD_NOT_INVOKE_CONSTRUCTOR %@ xxx.", newClassName] stringValue];
        }
        return @"OK";
    }
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
