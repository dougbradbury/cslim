#import "OCSMethodCaller.h"
#import "OCSException.h"
#import "OCSInvocation.h"

@implementation OCSMethodCaller

+(id) withInstance:(id) instance
      instanceName:(NSString*) instanceName
        methodName:(NSString*) methodName
           andArgs:(NSArray*) args {
    OCSMethodCaller* methodCaller = [self new];
    methodCaller.instance = instance;
    methodCaller.instanceName = instanceName;
    methodCaller.methodName = methodName;
    methodCaller.args = args;
    return methodCaller;
}

-(NSString*) call {
    if([self isInstanceNULL]) {
        return [[self instanceMissingException] stringValue];
    } else if([self isMethodMissingOnInstance]) {
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
    OCSInvocation* ocsInvocation = [OCSInvocation invocationWithInstance: self.instance
                                                              methodName: self.methodName
                                                            andArguments: self.args];
    return [ocsInvocation invoke];
}

-(BOOL) isInstanceNULL {
    return self.instance == NULL;
}

-(BOOL) isMethodMissingOnInstance {
    return ![self.instance respondsToSelector: [self selector]];
}

-(OCSException*) instanceMissingException {
    return [OCSException exceptionWithMessage: @"The instance %@. does not exist", self.instanceName];
}

-(OCSException*) methodMissingException {
    return [OCSException exceptionWithMessage: @"NO_METHOD_IN_CLASS %@ %@.",
            NSStringFromSelector([self selector]),
            NSStringFromClass([self.instance class])];
}

-(SEL) selector {
    return [OCSInvocation selectorForString: self.methodName andArgs: self.args];
}

@end
