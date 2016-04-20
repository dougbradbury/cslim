#include "SlimConnectionHandler.h"
#include "SlimListSerializer.h"

#include <stdlib.h>
#include <string.h>
#include <string.h>
#include <stdio.h>

struct SlimConnectionHandler
{
	com_func_t sendFunc;
	com_func_t recvFunc;
	void * comLink;
	handler_func_t slimHandlerFunc;
  void * slimHandler;
};

SlimConnectionHandler* SlimConnectionHandler_Create(com_func_t sendFunction, com_func_t recvFunction, void * comLink)
{
	SlimConnectionHandler* self = (SlimConnectionHandler*)malloc(sizeof(SlimConnectionHandler));
	memset(self, 0, sizeof(SlimConnectionHandler));
	self->sendFunc = sendFunction;
	self->recvFunc = recvFunction;
	self->comLink = comLink;
	return self;
}

void SlimConnectionHandler_Destroy(SlimConnectionHandler* self)
{
	free(self);
}

void SlimConnectionHandler_RegisterSlimMessageHandler(SlimConnectionHandler* self, void* handler, handler_func_t handlerFunc )
{
  self->slimHandler = handler;
	self->slimHandlerFunc = handlerFunc;
}

int read_size(SlimConnectionHandler* self)
{
	char size[7];
	int size_i = 0;
	char colon;
	memset(size, (size_t)0, (size_t)7);

	int receiveResult = self->recvFunc(self->comLink, size, 6);
	if (receiveResult == 6)
	{
		if ((self->recvFunc(self->comLink, &colon, 1)) == 1 && colon == ':')
		{
			size_i = atoi(size);
		}
	} else if (receiveResult == -1) {
		size_i = -1;;
	}
	return size_i;
}


int SlimConnectionHandler_Run(SlimConnectionHandler* self)
{

	if (self->sendFunc(self->comLink, "Slim -- V0.3\n", 13) == -1)
	{
		return -1;
	}

	char* message = NULL;
	while(1)
	{
		int size_i = read_size(self);

		if (size_i > 0)
		{
			free(message);
			message = (char*)malloc(size_i + (size_t)1);
			memset(message, 0, (size_t)size_i + (size_t)1);
			int numbytes = self->recvFunc(self->comLink, message, size_i);
			if (numbytes != size_i)
			{
				printf("did not receive right number of bytes.  %d expected but received %d\n", size_i, numbytes);
				break;
			}
			if (strcmp("bye", message) == 0)
			{
				break;
			}
			//execute and get response
			char* response_message = self->slimHandlerFunc(self->slimHandler, message);
			int response_length = (int)strlen(response_message);
			char * length_buffer = (char *)malloc((size_t)8);
			sprintf(length_buffer, "%06d:", response_length);
			int send_result = self->sendFunc(self->comLink, length_buffer, 7);
			free(length_buffer);
			send_result = self->sendFunc(self->comLink, response_message, response_length);
			SlimList_Release(response_message);
		} else if (size_i == -1)
		{
			break;
		}
	}
	free(message);
	fflush(stdout);
	return 0;

}
