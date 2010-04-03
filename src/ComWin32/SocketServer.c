#include "SocketServer.h"
#include <winsock2.h>
#include <stdio.h>

static void StartWSA(SocketServer* self);
static void BindToSocket(SocketServer* self);
static void Shutdown(SocketServer* self);
static void CreateSocket(SocketServer* self);
static void WaitForConnection(SocketServer* self);

//static local variables
struct SocketServer
{
	int (*handler)(int);
	SOCKET Socket;
	int itsPort;	
};

SocketServer* SocketServer_Create(void)
{
     SocketServer* self = (SocketServer*)malloc(sizeof(SocketServer));
     memset(self, 0, sizeof(SocketServer));
     return self;
}

void SocketServer_Destroy(SocketServer* self)
{
	Shutdown(self);
    free(self);
}

void SocketServer_register_handler(SocketServer* self, int (*handlerFunction)(int))
{
	self->handler = handlerFunction;
}

void serve(SocketServer* self, int socket)
{
	(*self->handler)(socket);
}

int SocketServer_Run(SocketServer* self, char * listening_port_number)
{
	self->itsPort = atoi(listening_port_number);
	StartWSA(self);
	CreateSocket(self);
	BindToSocket(self);
	WaitForConnection(self);	
	Shutdown(self);

    return 0;
}

void Shutdown(SocketServer* self)
{
	shutdown(self->Socket,SD_SEND);
	closesocket(self->Socket);
	WSACleanup();
}

void WaitForConnection(SocketServer* self)
{
	listen(self->Socket,1);

	SOCKET TempSock = SOCKET_ERROR;
	while(TempSock==SOCKET_ERROR)
	{
		TempSock = accept(self->Socket,NULL,NULL);
	}
	
	serve(self, TempSock);

	closesocket(TempSock);
}

void StartWSA(SocketServer* self)
{
	WSADATA WsaDat;
	if(WSAStartup(MAKEWORD(2,2),&WsaDat) != 0)
	{
		printf("WSA Initialization failed!");
	}
}

void CreateSocket(SocketServer* self)
{
	self->Socket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(self->Socket == INVALID_SOCKET)
	{
		printf("Socket creationg failed.");
	}
}

void BindToSocket(SocketServer* self)
{
	SOCKADDR_IN serverInf;
	serverInf.sin_family=AF_INET;
	serverInf.sin_addr.s_addr=INADDR_ANY;
	serverInf.sin_port=htons(self->itsPort);

	if(bind(self->Socket,(SOCKADDR*)(&serverInf),sizeof(serverInf)) == SOCKET_ERROR)
	{
		printf("Unable to bind socket!");
	}
}
