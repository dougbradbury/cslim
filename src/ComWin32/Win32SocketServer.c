#include <winsock.h>

#include <stdlib.h>
#include <memory.h>
#include <stdio.h>
#include "SocketServer.h"

struct SocketServer
{
	int (*handler)(int);
};

////////////////////////////////////////////////////////////////////////
// Prototypes

SOCKET SetUpListener(const char* pcAddress, int nPort);
SOCKET AcceptConnection(SOCKET ListeningSocket, sockaddr_in& sinRemote);
int ShutdownConnection(SOCKET sd);
void serve(SocketServer* self, int socket);

//// DoWinsock /////////////////////////////////////////////////////////
// The module's driver function -- we just call other functions and
// interpret their results.

int DoWinsock(SocketServer* self, const char* pcAddress, int nPort)
{
	// Begin listening for connections
	SOCKET ListeningSocket = SetUpListener(pcAddress, htons(nPort));
	if (ListeningSocket == INVALID_SOCKET) {
		return 3;
	}

	sockaddr_in sinRemote;
	SOCKET sd = AcceptConnection(ListeningSocket, sinRemote);
	if (sd == INVALID_SOCKET) {
		return 3;
	}
	
	serve(self, sd);
	if (0 == ShutdownConnection(sd)) {
		return 3;
	}
	return 1;		
}


//// SetUpListener /////////////////////////////////////////////////////
// Sets up a listener on the given interface and port, returning the
// listening socket if successful; if not, returns INVALID_SOCKET.

SOCKET SetUpListener(const char* pcAddress, int nPort)
{
	u_long nInterfaceAddr = inet_addr(pcAddress);
	if (nInterfaceAddr != INADDR_NONE) {
		SOCKET sd = socket(AF_INET, SOCK_STREAM, 0);
		if (sd != INVALID_SOCKET) {
			sockaddr_in sinInterface;
			sinInterface.sin_family = AF_INET;
			sinInterface.sin_addr.s_addr = nInterfaceAddr;
			sinInterface.sin_port = nPort;
			if (bind(sd, (sockaddr*)&sinInterface, 
					sizeof(sockaddr_in)) != SOCKET_ERROR) {
				listen(sd, 1);
				return sd;
			}
		}
	}

	return INVALID_SOCKET;
}


//// AcceptConnection //////////////////////////////////////////////////
// Waits for a connection on the given socket.	When one comes in, we
// return a socket for it.	If an error occurs, we return 
// INVALID_SOCKET.

SOCKET AcceptConnection(SOCKET ListeningSocket, sockaddr_in& sinRemote)
{
	int nAddrSize = sizeof(sinRemote);
	return accept(ListeningSocket, (sockaddr*)&sinRemote, &nAddrSize);
}

int ShutdownConnection(SOCKET sd)
{
    // Disallow any further data sends.  This will tell the other side
    // that we want to go away now.  If we skip this step, we don't
    // shut the connection down nicely.
    if (shutdown(sd, SD_SEND) == SOCKET_ERROR) {
        return 0;
    }

    // Receive any extra data still sitting on the socket.  After all
    // data is received, this call will block until the remote host
    // acknowledges the TCP control packet sent by the shutdown above.
    // Then we'll get a 0 back from recv, signalling that the remote
    // host has closed its side of the connection.
    char acReadBuffer[12];
    while (1) {
        int nNewBytes = recv(sd, acReadBuffer, 128, 0);
        if (nNewBytes == SOCKET_ERROR) {
            return 0;
        }
        else if (nNewBytes != 0) {
        }
        else {
            // Okay, we're done!
            break;
        }
    }

    // Close the socket.
    if (closesocket(sd) == SOCKET_ERROR) {
        return 0;
    }

    return 1;
}



SocketServer* SocketServer_Create(void)
{
  
	WSAData wsaData;
	int nCode;
	if ((nCode = WSAStartup(MAKEWORD(1, 1), &wsaData)) != 0) {
		return 0;
	}
	 SocketServer* self = (SocketServer*)malloc(sizeof(SocketServer));
	 memset(self, 0, sizeof(SocketServer));
	 return self;
}

void SocketServer_Destroy(SocketServer* self)
{
	// Shut Winsock back down and take off.
	WSACleanup();
	free(self);
}

void serve(SocketServer* self, int socket)
{
	(*self->handler)(socket);
}

int SocketServer_Run(SocketServer* self, char * listening_port_number)
{
	return DoWinsock(self, "127.0.0.1", atoi(listening_port_number));
}

void SocketServer_register_handler(SocketServer* self, int (*handlerFunction)(int))
{
	self->handler = handlerFunction;
}
