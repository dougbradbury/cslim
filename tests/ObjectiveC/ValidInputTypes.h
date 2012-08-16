#import <Foundation/Foundation.h>

@interface ValidInputTypes : NSObject

@property (nonatomic, strong) NSString* calledWith;
@property (readwrite, assign) BOOL callWithZeroArgsCalled;
@property (nonatomic, strong) NSString* firstArg;
@property (nonatomic, strong) NSString* secondArg;

@end
