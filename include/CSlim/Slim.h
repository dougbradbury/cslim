#ifndef D_Slim_H
#define D_Slim_H

#ifndef CPP_COMPILING
#ifdef __cplusplus
extern "C" {
#endif
#endif

#include "SlimConnectionHandler.h"

typedef struct Slim Slim;

Slim * Slim_Create();
void Slim_Destroy(Slim*);
char * Slim_HandleMessage(void* self, char * message);
int Slim_HandleConnection(Slim* self, void* comLink, com_func_send_t send, com_func_recv_t recv);

#ifndef CPP_COMPILING
#ifdef __cplusplus
}
#endif
#endif

#endif 
