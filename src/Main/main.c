#include "SocketServer.h"
#include "SlimConnectionHandler.h"
#include "TcpComLink.h"
#include <stdlib.h>
#include <string.h>
#include "SlimList.h"
#include "SlimListDeserializer.h"

char * temp_handle_slim_message(char * message);
//SlimExecutor executor;

int connection_handler(int socket)
{
	int result = 0;
	TcpComLink * comLink = TcpComLink_Create(socket);
	SlimConnectionHandler* connection = SlimConnectionHandler_Create(&TcpComLink_send, &TcpComLink_recv, (void*)comLink);
	SlimConnectionHandler_RegisterSlimMessageHandler(connection, &temp_handle_slim_message);

	result = SlimConnectionHandler_Run(connection);

	SlimConnectionHandler_Destroy(connection);
	TcpComLink_Destroy(comLink);
	
	return result;
}

int main(int ac, char** av)
{
//	executor = SlimExecutor_Create();
	SocketServer* server = SocketServer_Create();
	SocketServer_register_handler(server, &connection_handler);
		
	int result = SocketServer_Run(server, av[1]);
	
	SocketServer_Destroy(server);
//	SlimExecutor_Destroy(executor);
	
	return result;
}



char * temp_handle_slim_message(char * message)
{
	SlimList* instructions = SlimList_Deserialize(message);
//	SlimList results = SlimExecutor_execute(executor, instructions);
//	char * response = SlimList_Serialize(results);
//	SlimListDestroy(results);
	SlimList_Destroy(instructions);
		
	
	char * response = (char *)malloc(32);
	strcpy(response, "[000000:]");
	
	return response;
}
