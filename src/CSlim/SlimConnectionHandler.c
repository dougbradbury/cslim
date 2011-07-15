#include "SlimConnectionHandler.h"
#include <stdlib.h>
#include <string.h>
#include <string.h>
// #include <sys/types.h>
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
	int size_i = -1;
	char colon;
	memset(size, 0, 7);

	if (self->recvFunc(self->comLink, size, 6) == 6)
	{
		if ((self->recvFunc(self->comLink, &colon, 1)) == 1 && colon == ':')
		{
			size_i = atoi(size);
		}
	}
	return size_i;
}


int SlimConnectionHandler_Run(SlimConnectionHandler* self)
{
	char * message = (char*)malloc(3);
	message[0] = 0;
	int numbytes;

	if (self->sendFunc(self->comLink, "Slim -- V0.0\n", 13) == -1)
	{
		return -1;
	}

	while(1)
	{
		int size_i = read_size(self);
		if (size_i > 0)
		{
			free(message);
			message = (char*)malloc(size_i + 1);
			memset(message, 0, size_i + 1);
			numbytes = self->recvFunc(self->comLink, message, size_i);
      self->sendFunc(self->comLink, message, size_i);
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
			int response_length = strlen(response_message);
			int response_message_length = response_length + 7;
			char * response = (char *)malloc(response_message_length + 1);
			sprintf(response, "%06d:%s", response_length, response_message);
			free(response_message);
			int send_result = self->sendFunc(self->comLink, response, response_message_length);
			free(response);
			if ( send_result != response_message_length)
			{
        self->sendFunc(self->comLink, "Failure to send all data", 24);
				break;
			}
		}
	}
	free(message);
	fflush(stdout);
	return 0;

}
