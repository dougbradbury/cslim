#include "SocketServer.h"
#include "SlimConnectionHandler.h"
#include "TcpComLink.h"
#include <stdlib.h>
#include <string.h>
#include "SlimList.h"
#include "SlimListDeserializer.h"
#include "StatementExecutor.h"
#include "ListExecutor.h"
#include "TestSlim.h"
#include "SlimListSerializer.h"
extern void Division_Register(StatementExecutor* executor);
extern void Count_Register(StatementExecutor* executor);
extern void EmployeePayRecordsRow_Register(StatementExecutor* executor);

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

static StatementExecutor * statementExecutor;
static ListExecutor * listExecutor;
int main(int ac, char** av)
{
	statementExecutor = StatementExecutor_Create();
	StatementExecutor_AddFixture(statementExecutor, TestSlim_Register);
	StatementExecutor_AddFixture(statementExecutor, Division_Register);
	StatementExecutor_AddFixture(statementExecutor, Count_Register);
	StatementExecutor_AddFixture(statementExecutor, EmployeePayRecordsRow_Register);
	
	listExecutor = ListExecutor_Create(statementExecutor);
	
	SocketServer* server = SocketServer_Create();
	SocketServer_register_handler(server, &connection_handler);
		
	int result = SocketServer_Run(server, av[1]);
	
	SocketServer_Destroy(server);
	ListExecutor_Destroy(listExecutor);
	StatementExecutor_Destroy(statementExecutor);
	return result;
}



char * temp_handle_slim_message(char * message)
{
	SlimList* instructions = SlimList_Deserialize(message);
	SlimList* results = ListExecutor_Execute(listExecutor, instructions);
	char * response = SlimList_Serialize(results);
	SlimList_Destroy(results);
	SlimList_Destroy(instructions);
	return response;
}
