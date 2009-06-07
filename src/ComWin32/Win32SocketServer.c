#ifdef WIN_23
#include "SocketServer.h"
#include <stdlib.h>
#include <memory.h>
#include <stdio.h>

struct SocketServer
{
	int (*handler)(int);
};

SocketServer* SocketServer_Create(void)
{
     SocketServer* self = malloc(sizeof(SocketServer));
     memset(self, 0, sizeof(SocketServer));
     return self;
}

void SocketServer_Destroy(SocketServer* self)
{
    free(self);
}

void serve(SocketServer* self, int socket)
{
	(*self->handler)(socket);
}

int SocketServer_Run(SocketServer* self, char * listening_port_number)
{
	serve(self, new_fd);

    return 0;
}

#endif

