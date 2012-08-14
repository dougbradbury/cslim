#import <Foundation/Foundation.h>

@interface OCSException : NSObject

@property (nonatomic, strong) NSString* stringValue;

+(id) exceptionWithNSException:(NSException*) givenException;
+(id) exceptionWithMessage:(NSString*) message, ... NS_FORMAT_FUNCTION(1,2);

@end
