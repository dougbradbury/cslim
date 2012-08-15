#import "OCSMethodCaller.h"
#import "OCSException.h"
#import "OCSReturnValue.h"

@implementation OCSMethodCaller

+(id) withInstance:(id) instance
      instanceName:(NSString*) instanceName
        methodName:(NSString*) methodName
           andArgs:(NSArray*) updatedArgs {
    OCSMethodCaller* methodCaller = [self new];
    methodCaller.instance = instance;
    methodCaller.instanceName = instanceName;
    methodCaller.methodName = methodName;
    methodCaller.args = updatedArgs;
    return methodCaller;
}

-(NSString*) call {
    if([self instanceIsNULL]) {
        return [[self instanceMissingException] stringValue];
    } else if([self methodMissingOnInstance]) {
        return [[self methodMissingException] stringValue];
    } else {
        return [self attemptCallWithTryCatch];
    }
}

-(NSString*) attemptCallWithTryCatch {
    @try {
        return [self attemptCall];
    } @catch (NSException* e) {
        return [[OCSException exceptionWithNSException: e] stringValue];
    }
}

-(NSString*) attemptCall {
    id result;
    switch ([self.args count]) {
        case 0:
            result = [self.instance performSelector: [self selector]];
            break;
        case 1:
            result = [self.instance performSelector: [self selector] withObject: [self.args objectAtIndex: 0]];
            break;
        default:
            result = [self.instance performSelector: [self selector] withObject: self.args];
            break;
    }
    return [OCSReturnValue forObjectOrPrimitive: result andMethodSignature: [self signature]];
}

-(BOOL) instanceIsNULL {
    return self.instance == NULL;
}

-(BOOL) methodMissingOnInstance {
    return ![self.instance respondsToSelector: [self selector]];
}

-(OCSException*) instanceMissingException {
    return [OCSException exceptionWithMessage: @"The instance %@. does not exist", self.instanceName];
}

-(OCSException*) methodMissingException {
    return [OCSException exceptionWithMessage: @"NO_METHOD_IN_CLASS %@[%i] TestSlim.",
            self.methodName,
            (int)[self.args count]];
}

-(NSMethodSignature*) signature {
    return [self.instance methodSignatureForSelector: [self selector]];
}

-(SEL) selector {
    if ([self.args count] == 0) {
        return NSSelectorFromString(self.methodName);
    } else {
        return NSSelectorFromString([NSString stringWithFormat:@"%@:", self.methodName]);
    }
}

@end
