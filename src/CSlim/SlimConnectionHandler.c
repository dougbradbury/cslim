#include "SlimConnectionHandler.h"
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <sys/types.h>
#include <stdio.h>

//static local variables

struct SlimConnectionHandler
{
	com_func_t sendFunc;
	com_func_t recvFunc;
	void * comLink;
	char * (*slimHandler)(char *);
};

SlimConnectionHandler* SlimConnectionHandler_Create(com_func_t sendFunction, com_func_t recvFunction, void * comLink)
{
	SlimConnectionHandler* self = malloc(sizeof(SlimConnectionHandler));
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

void SlimConnectionHandler_registerSlimMessageHandler(SlimConnectionHandler* self, char * (*handler)(char *) )
{
	self->slimHandler = handler;
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


int SlimConnectionHandler_run(SlimConnectionHandler* self)
{
	char * message = malloc(3);
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
			message = malloc(size_i + 1);
			memset(message, 0, size_i + 1);

		    if ((numbytes = self->recvFunc(self->comLink, message, size_i)) == -1) 
		        break;
			if (strcmp("bye", message) == 0)
				break;
			
			//execute and get response
			char* response_message = self->slimHandler(message);
			int response_length = strlen(response_message);
			char * response = malloc(response_length + 7);
			sprintf(response, "%06d:%s", response_length, response_message);
			free(response_message);
			int send_result = self->sendFunc(self->comLink, response, response_length + 7);
			free(response);
			if ( send_result == -1)
				break;
		}
	}
	free(message);
	return 0;

}
