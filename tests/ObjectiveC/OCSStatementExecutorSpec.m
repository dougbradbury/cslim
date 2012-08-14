#import <SpecKit/SpecKit.h>
#import "OCSStatementExecutor.h"

SpecKitContext(OCSStatementExecutorSpec) {
    
    Describe(@"+sharedExecutor", ^{
        
        It(@"returns an executor", ^{
            [ExpectObj([OCSStatementExecutor sharedExecutor]) toExist];
        });
        
        It(@"returns the same executor", ^{
            [ExpectObj([OCSStatementExecutor sharedExecutor]) toBeEqualTo: [OCSStatementExecutor sharedExecutor]];
        });
        
    });
    
}
