#import "OCSException.h"

@implementation OCSException

+(id) exceptionWithNSException:(NSException*) givenException {
    OCSException* exception = [self new];
    exception.stringValue = [NSString stringWithFormat: @"__EXCEPTION__:message:<<%@ %@>>",
                             [givenException name],
                             [givenException reason]];
    
    return exception;
}

+(id) exceptionWithMessage:(NSString*) message, ... NS_FORMAT_FUNCTION(1,2) {
    va_list args;
    va_start(args, message);
    OCSException* exception = [self new];
    NSString *formattedMessage = [[NSString alloc] initWithFormat: message arguments:args];
    exception.stringValue = [NSString stringWithFormat: @"__EXCEPTION__:message:<<%@>>", formattedMessage];
    va_end(args);
    return exception;
}

@end
