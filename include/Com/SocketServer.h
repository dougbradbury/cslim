#ifndef D_SocketServer_H
#define D_SocketServer_H

///////////////////////////////////////////////////////////////////////////////
//
//  SocketServer is responsible for ...
//
///////////////////////////////////////////////////////////////////////////////

#ifndef CPP_COMPILING
#ifdef __cplusplus
extern "C" {
#endif
#endif

typedef struct SocketServer SocketServer;

SocketServer* SocketServer_Create(void);
void          SocketServer_Destroy(SocketServer*);
extern int    SocketServer_Run(SocketServer* self, char* port);
extern void   SocketServer_register_handler(SocketServer* self, int (*handlerFunction)(int));

#ifndef CPP_COMPILING
#ifdef __cplusplus
}
#endif
#endif

#endif // D_SocketServer_H
