#ifndef D_Slim_H
#define D_Slim_H
#include "SlimConnectionHandler.h"
///////////////////////////////////////////////////////////////////////////////
//
//  Slim is responsible for ...
//
///////////////////////////////////////////////////////////////////////////////

typedef struct Slim Slim;

// typedef int(*com_func_t)(void * handle, char * msg, int length);

// Slim* Slim_Create(com_func_t sendFunction, com_func_t recvFunction, void * comLink);
Slim * Slim_Create();
void Slim_Destroy(Slim*);
char * Slim_HandleMessage(void* self, char * message);
int Slim_HandleConnection(Slim* self, void* comLink, com_func_t send, com_func_t recv);
#endif  // D_Slim_H
