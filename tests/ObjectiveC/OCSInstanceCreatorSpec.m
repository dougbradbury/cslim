#import <SpecKit/SpecKit.h>
#import "OCSInstanceCreator.h"
#import "TestSlim.h"

SpecKitContext(OCSInstanceCreatorSpec) {
    
    __block OCSInstanceCreator* instanceCreator;
    __block NSMutableArray* args;
    
    Describe(@"creating instances", ^{
        
        BeforeEach(^{
            args = [NSMutableArray array];
        });
        
        It(@"makes an instance of a class with no arguments", ^{
            instanceCreator = [OCSInstanceCreator instanceWithClassName: @"TestSlim"
                                                                andArgs: args];
            
            id test_slim_instance = [instanceCreator create];
            
            [ExpectObj(test_slim_instance) toBeKindOfClass: [TestSlim class]];
        });
        
        It(@"makes an instance of a class with 1 argument", ^{
            [args addObject: @"starting param"];
            instanceCreator = [OCSInstanceCreator instanceWithClassName: @"TestSlim"
                                                                andArgs: args];
            
            TestSlim* test_slim_instance = [instanceCreator create];
            
            [ExpectObj(test_slim_instance.calledWithStringArg) toBeEqualTo: @"starting param"];
        });
        
        It(@"makes an instance of a class with 2 arguments", ^{
            [args addObject: @"first param"];
            [args addObject: @"second param"];
            instanceCreator = [OCSInstanceCreator instanceWithClassName: @"TestSlim"
                                                                andArgs: args];
            
            TestSlim* test_slim_instance = [instanceCreator create];
            
            [ExpectObj(test_slim_instance.calledWithFirstStringArg) toBeEqualTo: @"first param"];
            [ExpectObj(test_slim_instance.calledWithSecondStringArg) toBeEqualTo: @"second param"];
        });
        
        It(@"returns an error if the wrong number of arguments is passed for make (1 for 0)", ^{
            [args addObject: @"starting param"];
            instanceCreator = [OCSInstanceCreator instanceWithClassName: @"TestSlimTwo"
                                                                andArgs: args];
            
            [instanceCreator create];
            
            [ExpectObj([instanceCreator result]) toBeEqualTo: @"__EXCEPTION__:message:<<COULD_NOT_INVOKE_CONSTRUCTOR TestSlimTwo[1].>>"];
        });
        
        It(@"returns an error if an invalid class is given", ^{
            instanceCreator = [OCSInstanceCreator instanceWithClassName: @"NoSuchClass"
                                                                andArgs: NULL];
            
            [instanceCreator create];
            
            [ExpectObj([instanceCreator result]) toBeEqualTo: @"__EXCEPTION__:message:<<NO_CLASS NoSuchClass.>>"];
        });
        
    });
    
}
