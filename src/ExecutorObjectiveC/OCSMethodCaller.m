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
    [self setupInvocation];
    [invocation invoke];
    return [OCSReturnValue forInvocation: invocation];
}

-(void) setupInvocation {
    NSMethodSignature* methodSignature = [self.instance methodSignatureForSelector: [self selector]];
    invocation = [NSInvocation invocationWithMethodSignature: methodSignature];
    [invocation setTarget: self.instance];
    [invocation setSelector: [self selector]];
    [self setArgsOnInvocation];
}

-(void) setArgsOnInvocation {
    for (int i=0; i<[[self argsForInvocation] count]; i++) {
        NSString* arg = [[self argsForInvocation] objectAtIndex: i];
        [invocation setArgument: &arg atIndex: i + 2];
    }
}

-(NSArray*) argsForInvocation {
    if ([[self splitMethodName] count] > 1 || [self.args count] == 1) {
        return self.args;
    } else if ([[self splitMethodName] count] == 1 && [self.args count] > 1) {
        return [NSArray arrayWithObjects: self.args, nil];
    }
    return [NSArray array];
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
    if ([[self splitMethodName] count] > 1) {
        NSString* multiMethodName = [[self splitMethodName] componentsJoinedByString: @":"];
        multiMethodName = [multiMethodName stringByAppendingString: @":"];
        return NSSelectorFromString(multiMethodName);
    } else {
        if ([self.args count] == 0) {
            return NSSelectorFromString(self.methodName);
        } else {
            return NSSelectorFromString([NSString stringWithFormat:@"%@:", self.methodName]);
        }
    }
}

-(NSArray*) splitMethodName {
    return [self.methodName componentsSeparatedByString: @","];
}

@end
