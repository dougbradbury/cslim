#import <Foundation/Foundation.h>

@interface ValidInputTypes : NSObject {
    NSString* calledWith;
    BOOL callWithZeroArgsCalled;
}
@property (nonatomic, retain) NSString* calledWith;
@property (readwrite, assign) BOOL callWithZeroArgsCalled;

@end
