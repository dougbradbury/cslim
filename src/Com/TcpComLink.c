#include "TcpComLink.h"
#include <stdlib.h>
#include <memory.h>
#include <sys/socket.h>

//static local variables
struct TcpComLink
{
    int socket;
};

TcpComLink* TcpComLink_Create(int socket)
{
	TcpComLink* self = malloc(sizeof(TcpComLink));
	memset(self, 0, sizeof(TcpComLink));
	self->socket = socket;
	return self;
}

void TcpComLink_Destroy(TcpComLink* self)
{
    free(self);
}


int TcpComLink_send(void * voidSelf, char * msg, int length)
{
	TcpComLink * self = (TcpComLink *)voidSelf;
	return send(self->socket, msg, length, 0);
}
int TcpComLink_recv(void * voidSelf, char * buffer, int length)
{
	TcpComLink * self = (TcpComLink *)voidSelf;
	return recv(self->socket, buffer, length, 0);
}
