#include "SocketServer.h"
#include "SlimConnectionHandler.h"
#include "TcpComLink.h"
#include <stdlib.h>
#include <string.h>

char * temp_handle_slim_message(char * message);

int connection_handler(int socket)
{
	int result = 0;
	TcpComLink * comLink = TcpComLink_Create(socket);
	SlimConnectionHandler* connection = SlimConnectionHandler_Create(&TcpComLink_send, &TcpComLink_recv, (void*)comLink);
	SlimConnectionHandler_registerSlimMessageHandler(connection, &temp_handle_slim_message);

	result = SlimConnectionHandler_run(connection);

	SlimConnectionHandler_Destroy(connection);
	TcpComLink_Destroy(comLink);
	
	return result;
}

int main(int ac, char** av)
{
	SocketServer* server = SocketServer_Create();
	SocketServer_register_handler(server, &connection_handler);
		
	int result = SocketServer_Run(server, av[1]);
	
	SocketServer_Destroy(server);
	
	return result;
}



char * temp_handle_slim_message(char * message)
{
	char * response = malloc(32);
	strcpy(response, "[000000:]");
	return response;
}
