#import <OCDSpec2/OCDSpec2.h>
#import "OCSException.h"

OCDSpec2Context(OCSExceptionSpec) {
    
    __block OCSException* exception;
    
    Describe(@"+exceptionWithNSException", ^{
        
        It(@"has a string value", ^{
            NSException* nsException = [NSException exceptionWithName: @"The Name"
                                                               reason: @"The Reason"
                                                             userInfo: NULL];
            exception = [OCSException exceptionWithNSException: nsException];
            
            [ExpectObj([exception stringValue]) toBeEqualTo: @"__EXCEPTION__:message:<<The Name The Reason>>"];
        });
        
    });

    Describe(@"+exceptionWithMessage", ^{
        
        It(@"has a string value for a simple string", ^{
            exception = [OCSException exceptionWithMessage: @"Some Message"];
            
            [ExpectObj([exception stringValue]) toBeEqualTo: @"__EXCEPTION__:message:<<Some Message>>"];
        });

        It(@"has a string value for a string with one interpolated value", ^{
            exception = [OCSException exceptionWithMessage: @"Message %i", 123];
            
            [ExpectObj([exception stringValue]) toBeEqualTo: @"__EXCEPTION__:message:<<Message 123>>"];
        });

        It(@"has a string value for a string with two interpolated values", ^{
            exception = [OCSException exceptionWithMessage: @"Message %i %@", 123, @"Count"];
            
            [ExpectObj([exception stringValue]) toBeEqualTo: @"__EXCEPTION__:message:<<Message 123 Count>>"];
        });

    });

}
