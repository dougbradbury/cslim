#include <stdio.h>
#include <winsock.h>
#include <stdlib.h>

#include "TcpComLink.h"
#include <memory.h>

//static local variables
struct TcpComLink
{
    int socket;
};

TcpComLink* TcpComLink_Create(int socket)
{
	TcpComLink* self = (TcpComLink*)malloc(sizeof(TcpComLink));
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

	int nSentBytes = 0;
	while (nSentBytes < length) {
		int nTemp = send(self->socket, msg + nSentBytes,
				length - nSentBytes, 0);
		if (nTemp > 0) {
			nSentBytes += nTemp;
		}
		else if (nTemp == SOCKET_ERROR) {
			return nSentBytes;
		}
		else {
			// Client closed connection before we could reply to
			// all the data it sent, so bomb out early.
			return 0;
		}
	}
	return nSentBytes;
}

int TcpComLink_recv(void * voidSelf, char * buffer, int length)
{
	TcpComLink * self = (TcpComLink *)voidSelf;
	return recv(self->socket, buffer, length, 0);
}

