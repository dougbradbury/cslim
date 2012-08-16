#import "OCSInvocation.h"
#import "OCSReturnValue.h"

@implementation OCSInvocation

+(id) invocationWithInstance: (id) givenInstance
                  methodName: (NSString*) givenMethodName
                andArguments: (NSArray*) givenArgs {
    return [[OCSInvocation alloc] initWithInstance: givenInstance
                                        methodName: givenMethodName
                                      andArguments: givenArgs];
}

-(id) initWithInstance: (id) givenInstance
            methodName: (NSString*) givenMethodName
          andArguments: (NSArray*) givenArgs  {
    self.args = givenArgs;
    self.methodName = givenMethodName;
    self.instance = givenInstance;
    [self buildInvocation];
    return self;
}

-(NSString*)invoke {
    [self.invocation invoke];
    return [OCSReturnValue forInvocation: self.invocation];
    
}

- (void)buildInvocation {
    NSMethodSignature* methodSignature = [self.instance methodSignatureForSelector: [self selector]];
    self.invocation = [NSInvocation invocationWithMethodSignature: methodSignature];
    [self.invocation setTarget: self.instance];
    [self.invocation setSelector: [self selector]];
    [self setArgsOnInvocation];
}

-(void) setArgsOnInvocation {
    for (int i=0; i<[[self argsForInvocation] count]; i++) {
        NSString* arg = [[self argsForInvocation] objectAtIndex: i];
        [self.invocation setArgument: &arg atIndex: i + 2];
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

-(SEL) selector {
    return [OCSInvocation selectorForString: self.methodName andArgs: self.args];
}

-(NSArray*) splitMethodName {
    return [self.methodName componentsSeparatedByString: @","];
}

+(SEL) selectorForString:(NSString*) methodName andArgs:(NSArray*) args {
    if ([[self splitMethodName: methodName] count] > 1) {
        NSString* multiMethodName = [[self splitMethodName: methodName] componentsJoinedByString: @":"];
        multiMethodName = [multiMethodName stringByAppendingString: @":"];
        return NSSelectorFromString(multiMethodName);
    } else {
        if ([args count] == 0) {
            return NSSelectorFromString(methodName);
        } else {
            return NSSelectorFromString([NSString stringWithFormat:@"%@:", methodName]);
        }
    }
}

+(NSArray*) splitMethodName:(NSString *) methodName {
    return [methodName componentsSeparatedByString: @","];
}
@end