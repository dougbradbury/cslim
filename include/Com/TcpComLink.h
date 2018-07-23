#ifndef D_TcpComLink_H
#define D_TcpComLink_H

///////////////////////////////////////////////////////////////////////////////
//
//  TcpComLink is responsible for ...
//
///////////////////////////////////////////////////////////////////////////////

#ifndef CPP_COMPILING
#ifdef __cplusplus
extern "C" {
#endif
#endif

typedef struct TcpComLink TcpComLink;

TcpComLink* TcpComLink_Create(int socket);
void TcpComLink_Destroy(TcpComLink*);
int TcpComLink_send(void * voidSelf, char * msg, int length);
int TcpComLink_recv(void * voidSelf, char * buffer, int length);

#ifndef CPP_COMPILING
#ifdef __cplusplus
}
#endif
#endif

#endif  // D_TcpComLink_H
