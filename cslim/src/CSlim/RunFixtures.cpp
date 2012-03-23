#include "ListExecutor.h"
#include "SlimConnectionHandler.h"
#include "SlimList.h"
#include "SocketServer.h"
#include "StatementExecutor.h"
#include "TcpComLink.h"

#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>

#include <string.h>

extern void AddFixtures(Slim::StatementExecutor* executor);

namespace
{
  std::string handle_slim_message(char const* message, Slim::ListExecutor& listExecutor)
  {
    std::unique_ptr<Slim::SlimList> instructions(Slim::SlimList::Deserialize(message));
    std::unique_ptr<Slim::SlimList> results(listExecutor.Execute(instructions.get()));
    return Slim::SlimList::Serialize(results.get());
  }

  int connection_handler(SOCKET socket, Slim::ListExecutor& listExecutor)
  {
    std::unique_ptr<Slim::TcpComLink> comLink(new Slim::TcpComLink(socket));
    std::unique_ptr<Slim::ConnectionHandler> connection(
      new Slim::ConnectionHandler(
      boost::bind(&Slim::TcpComLink::Send, comLink.get(), _1, _2),
      boost::bind(&Slim::TcpComLink::Receive, comLink.get(), _1, _2)));

    connection->RegisterSlimMessageHandler(boost::bind(&handle_slim_message, _1, listExecutor));
    return connection->Run();
  }
}

int RunFixtures(char* port)
{
  // Setting up fixtures
  Slim::StatementExecutor statementExecutor;
  AddFixtures(&statementExecutor);

  Slim::ListExecutor listExecutor(&statementExecutor);
  std::unique_ptr<Slim::SocketServer> server(new Slim::SocketServer());
  server->RegisterHandler(boost::bind(&connection_handler, _1, listExecutor));
  server->Run(boost::lexical_cast<int>(port));

  return 0;
}





