#import <Cocoa/Cocoa.h>
#include <CSlim/Slim.h>
#include <CSlim/SocketServer.h>
#include <CSlim/SlimConnectionHandler.h>
#include <CSlim/TcpComLink.h>

int main(int argc, const char * argv[]) {
    return NSApplicationMain(argc, argv);
}

Slim * slim;

int connection_handler(int socket) {
    int result = 0;
    TcpComLink * comLink = TcpComLink_Create(socket);
    
    result = Slim_HandleConnection(slim, (void*)comLink, &TcpComLink_send, &TcpComLink_recv);
    
    TcpComLink_Destroy(comLink);
    
    return result;
}

const char * slimPortArg() {
    NSArray *args = [[NSProcessInfo processInfo] arguments];
    const char * port = [ args[1] cStringUsingEncoding:NSASCIIStringEncoding] ;
    return port;
}

int runSlimSocketServer(const char * arg) {
    slim = Slim_Create();
    SocketServer* server = SocketServer_Create();
    SocketServer_register_handler(server, &connection_handler);
    
    int result = SocketServer_Run(server, (char*)arg);
    
    SocketServer_Destroy(server);
    Slim_Destroy(slim);
    return result;
}

void runSlimServer() {
    dispatch_async(dispatch_get_global_queue(QOS_CLASS_DEFAULT, 0), ^{
        const char * slimPort = slimPortArg();
        int result = runSlimSocketServer(slimPort);
        exit(result);
    });
}



@interface OCSlimNSApplicationRunner : NSApplication
@end

@implementation OCSlimNSApplicationRunner

-(id) init {
    if (self = [super init]) {
        runSlimServer();
    }
    return self;
}

@end
