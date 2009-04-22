#ifndef D_SlimConnectionHandler_H
#define D_SlimConnectionHandler_H

///////////////////////////////////////////////////////////////////////////////
//
//  SlimConnectionHandler is responsible for ...
//
///////////////////////////////////////////////////////////////////////////////

typedef struct SlimConnectionHandler SlimConnectionHandler;

typedef int(*com_func_t)(void * handle, char * msg, int length);

SlimConnectionHandler* SlimConnectionHandler_Create(com_func_t sendFunction, com_func_t recvFunction, void * comLink);
void SlimConnectionHandler_Destroy(SlimConnectionHandler*);
void SlimConnectionHandler_VirtualFunction_impl(SlimConnectionHandler*);
int SlimConnectionHandler_run(SlimConnectionHandler*);
void SlimConnectionHandler_registerSlimMessageHandler(SlimConnectionHandler*, char * (*handler)(char *) );

#endif  // D_SlimConnectionHandler_H
