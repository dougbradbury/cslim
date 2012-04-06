// Fitnesse Slim plugin port for VC++ 2010
// 
// Copyright (C) 2012  Chih C. Von
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

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





